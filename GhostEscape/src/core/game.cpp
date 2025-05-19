#include "game.h"
#include "../scene_main.h"


void Game::clean()
{
    asset_store_.unload();

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

void Game::drawGrid(glm::vec2 start, glm::vec2 end, float grid_w, float grid_h, SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(renderer_, color.a, color.g, color.b, color.a);
    for(float x = start.x; x <= end.x; x += grid_w)
    {
        SDL_RenderLine(renderer_, x, start.y, x, end.y);
    }

    for(float y = start.y; y <= end.y; y += grid_h)
    {
        SDL_RenderLine(renderer_, start.x, y, end.x, y);
    }
    SDL_SetRenderDrawColorFloat(renderer_, 0, 0, 0, 1);
}

void Game::drawBoundary(glm::vec2 start, glm::vec2 end, int width, SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(renderer_, color.a, color.g, color.b, color.a);
    for(int i = 0; i < width; ++i)
    {
        SDL_FRect rect = {start.x - i, start.y - i, end.x - start.x + 2 * i, end.y - start.y + 2 * i};
        SDL_RenderRect(renderer_, &rect);
    }
    SDL_SetRenderDrawColorFloat(renderer_, 0, 0, 0, 1);
}

void Game::changeScene(Scene *scene)
{
    if(current_scene_)
    {
        current_scene_->clean();
        delete current_scene_;
    }
    current_scene_ = scene;
    current_scene_->init();
}

void Game::handleEvent()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_EVENT_QUIT:
            is_running_ = false;
            break;
            default:
            current_scene_->handleEvent(event);
            break;
        }
    }
}

void Game::update(float delta_time)
{
    current_scene_->update(delta_time);
}

void Game::render()
{
    SDL_SetRenderDrawColorFloat(renderer_, 0,0,0,1);
    SDL_RenderClear(renderer_);
    current_scene_->render();
    SDL_RenderPresent(renderer_);
}

void Game::init(const std::string &tittle, int window_w, int window_h, int fps)
{
    screen_size_.x = window_w;
    screen_size_.y = window_h;
    FPS_ = fps;

    // SDL初始化（SDL3默认初始化图片，不用手动处理）
    if(!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());
        exit(-1);
    }
    if(Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) != (MIX_INIT_OGG | MIX_INIT_MP3))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_Init failed: %s", SDL_GetError());
        exit(-1);
    }
    if(!Mix_OpenAudio(0, nullptr))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_OpenAudio failed: %s", SDL_GetError());
        exit(-1);
    }
    if(!TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_Init failed: %s", SDL_GetError());
        exit(-1);
    }

    // 创建窗口，渲染器
    if(!SDL_CreateWindowAndRenderer(tittle.c_str(), window_w, window_h, SDL_WINDOW_RESIZABLE, &window_, &renderer_))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        exit(-1);
    }

    // // 设置逻辑分辨率
    // SDL_SetRenderLogicalPresentation(renderer_,  window_w, window_h, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // 创建字体渲染引擎
    text_engine_ = TTF_CreateRendererTextEngine(renderer_);
    if(!text_engine_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_CreateRendererTextEngine failed: %s", SDL_GetError());
        exit(-1);
    }

    // 加载资源
    asset_store_.load(renderer_, "assets");

    // 创建场景 TODO
    current_scene_ = new SceneMain;
    current_scene_->init();
}


void Game::run()
{
    SDL_Log("Game::run()");

    uint64_t last_frame_time = SDL_GetTicksNS();
    uint64_t delta_time = 0;
    uint64_t frame_interval = static_cast<int>(1e9) / FPS_;
    while(is_running_)
    {
        // 事件处理
        handleEvent();

        // 更新数据
        update(static_cast<float>(delta_time / 1e9));

        // 渲染画面
        render();

        // 帧率控制
        uint64_t cur_frame_time = SDL_GetTicksNS();
        delta_time = (cur_frame_time - last_frame_time);
        last_frame_time = cur_frame_time;
        if(delta_time < frame_interval)
        {
            SDL_DelayNS(frame_interval - delta_time);
            delta_time = frame_interval;
        }
    }
}

