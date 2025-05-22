#include "game.h"
#include "../scene_main.h"
#include "../scene_tittle.h"

void Game::handleEvent()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_EVENT_QUIT: is_running_ = false; break;
            case SDL_EVENT_WINDOW_RESIZED: updateWindowScale(); break;
            default: current_scene_->handleEvent(event); break;
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

void Game::controlFrameDelta()
{
    uint64_t cur_frame_time = SDL_GetTicksNS();
    delta_time_ = (cur_frame_time - last_frame_time_);
    last_frame_time_ = cur_frame_time;
    if(delta_time_ < frame_interval_)
    {
        SDL_DelayNS(frame_interval_ - delta_time_);
        delta_time_ = frame_interval_;
    }
}

void Game::updateWindowScale()
{
    int window_w = 0, window_h = 0;
    SDL_GetWindowSize(window_, &window_w, &window_h);
    // 设置屏幕宽高比不变
    SDL_SetWindowAspectRatio(window_, screen_size_.x / screen_size_.y, screen_size_.x / screen_size_.y);
    // 如果是全屏那么宽高比会改变，所以设置y轴缩放比例
    window_scale_.x = static_cast<float>(window_w) / screen_size_.x;    
    window_scale_.y = static_cast<float>(window_h) / screen_size_.y;
    SDL_SyncWindow(window_);
}

void Game::run()
{
    SDL_Log("Game::run()");
    while(is_running_)
    {
        if(scene_to_change_)
        {
            changeScene(scene_to_change_);
            scene_to_change_ = nullptr;
        }

        // 事件处理
        handleEvent();
        // 更新数据
        update(static_cast<float>(delta_time_ / 1e9));
        // 渲染画面
        render();
        // 帧率控制
        controlFrameDelta();
    }
}

void Game::clean()
{
    asset_store_.unload();
    TTF_DestroyRendererTextEngine(text_engine_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

void Game::init(const std::string &tittle, int window_w, int window_h, int fps)
{
    screen_size_.x = static_cast<float>(window_w);
    screen_size_.y =  static_cast<float>(window_h);
    FPS_ = fps;
    frame_interval_ = static_cast<int>(1e9) / FPS_;

    // 初始化随机数生成器
    random_gen_ = std::mt19937(std::random_device{}());

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

    // 设置逻辑分辨率 TODO
    SDL_SetRenderLogicalPresentation(renderer_,  window_w, window_h, SDL_LOGICAL_PRESENTATION_LETTERBOX);

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
    current_scene_ = new SceneTittle;
    current_scene_->init();
}



void Game::renderGrid(glm::vec2 start, glm::vec2 end, float grid_w, float grid_h, SDL_FColor color)
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

void Game::renderBoundary(glm::vec2 start, glm::vec2 end, int width, SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(renderer_, color.a, color.g, color.b, color.a);
    for(int i = 0; i < width; ++i)
    {
        SDL_FRect rect = {start.x - i, start.y - i, end.x - start.x + 2 * i, end.y - start.y + 2 * i};
        SDL_RenderRect(renderer_, &rect);
    }
    SDL_SetRenderDrawColorFloat(renderer_, 0, 0, 0, 1);
}

void Game::renderFillRect(glm::vec2 position, glm::vec2 size, SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(renderer_, color.r, color.g, color.b, color.a);
    SDL_FRect rect = {position.x, position.y, size.x, size.y};
    SDL_RenderFillRect(renderer_, &rect);
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

glm::vec2 Game::getRandomVec2(const glm::vec2 &begin, const glm::vec2 &end)
{
    float x = getRandom(begin.x, end.x);
    float y = getRandom(begin.y, end.y);
    return glm::vec2(x, y);
}

SDL_MouseButtonFlags Game::getMouseState(glm::vec2 &mouse_position)
{
    auto mouse_button = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    mouse_position /= window_scale_;
    return mouse_button;
}
