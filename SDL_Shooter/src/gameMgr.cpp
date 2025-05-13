#include "gameMgr.h"
#include "resourcesMgr.h"
#include "sceneMain.h"
#include <fstream>

GameMgr::GameMgr()
{
    // 初始化资源
    init();
}
GameMgr::~GameMgr()
{
    // 资源释放
    clean();
}

void GameMgr::init()
{
    // SDL初始化:图片，音频，字体
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL init failed: %s", SDL_GetError());
        exit(-1);
    }
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image init failed: %s", SDL_GetError());
        exit(-1);
    }
    if(Mix_Init(MIX_INIT_OGG) != (MIX_INIT_OGG)
    || Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer init failed: %s", SDL_GetError());
        exit(-1);
    }
    Mix_AllocateChannels(32);
    if(TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf init failed: %s", SDL_GetError());
        exit(-1);
    }

    // 创建窗口，渲染器
    window = SDL_CreateWindow(window_tittle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                window_w, window_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "create window failed: %s", SDL_GetError());
        exit(-1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "create renderer failed: %s", SDL_GetError());
        exit(-1);
    }

    // 设置逻辑分辨率，用于全屏缩放
    SDL_RenderSetLogicalSize(renderer, window_w, window_h);

    // 加载资源
    if(!ResMgr::getInstance().load(renderer, "assets")) exit(-1);
}

void GameMgr::clean()
{
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);

    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int GameMgr::run()
{
    uint64_t freq = SDL_GetPerformanceFrequency();
    uint64_t last_tick = SDL_GetPerformanceCounter();
    double delta_time = 0;

    // 场景初始化,不能在构造,因为场景需要用到GameMgr,那时还未初始化完成
    current_scene = new SceneMain;
    current_scene->enter();
    loadGame();

    // 主循环
    while(!is_quit)
    {
        // 事件处理
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                is_quit = true;
                break;
            }
            else
            {
                current_scene->handleEvent(event);
            }
        }
        
        // 数据更新
        current_scene->update(delta_time);

        // 渲染纹理
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);
        renderBackground();
        current_scene->render();
        SDL_RenderPresent(renderer);


        uint64_t cur_tick = SDL_GetPerformanceCounter();
        delta_time = static_cast<double>(cur_tick - last_tick) / static_cast<double>(freq);
        last_tick = cur_tick;
        if(delta_time < 1.0 / FPS)
        {
            SDL_Delay(static_cast<uint32_t>((1.0 / FPS - delta_time) * 1000));
            delta_time = 1.0 / FPS;
        }
    }

    saveGame();
    return 0;
}

void GameMgr::switchScene(Scene* scene)
{
    if(current_scene)
    {
        current_scene->exit();
        delete current_scene;
    }
    current_scene = scene;
    current_scene->enter();
}

void GameMgr::setRankingList(const std::string& name, int score)
{
    ranking_list.insert({score, name});
    if(ranking_list.size() >= 10) ranking_list.erase(--ranking_list.end());
}

SDL_Point GameMgr::renderText(TTF_Font* font, const std::string& text, double pos_x_ratio, double pos_y_ratio, bool is_left)
{
    SDL_Surface* suf = TTF_RenderUTF8_Solid(font, text.c_str(), {255,255,255,255});
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, suf);
    SDL_Rect rect = {static_cast<int>(is_left ? window_w * pos_x_ratio : (window_w - suf->w) * (1.0 - pos_x_ratio)),
                    static_cast<int>(window_h * pos_y_ratio), 
                    suf->w, suf->h};
    SDL_RenderCopy(renderer, tex, nullptr, &rect);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(suf);
    return {rect.x + rect.w, rect.y};
}

SDL_Point GameMgr::renderTextCenter(TTF_Font* font, const std::string& text, double pos_y_ratio)
{
    SDL_Surface* suf = TTF_RenderUTF8_Solid(font, text.c_str(), {255,255,255,255});
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, suf);
    SDL_Rect rect = {static_cast<int>(window_w / 2 - suf->w / 2),
                    static_cast<int>(window_h * pos_y_ratio), 
                    suf->w, suf->h};
    SDL_RenderCopy(renderer, tex, nullptr, &rect);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(suf);
    return {rect.x + rect.w, rect.y};
}


void GameMgr::renderBackground()
{
    static SDL_Texture* tex_nearstar = ResMgr::getInstance().find_texture(ResID::Tex_StarsA); 
    static SDL_Texture* tex_farstar = ResMgr::getInstance().find_texture(ResID::Tex_StarsB); 
    int nearstar_w = 0, nearstar_h = 0;
    int farstar_w = 0, farstar_h = 0;
    if(nearstar_w == 0)
    {
        SDL_QueryTexture(tex_nearstar, nullptr, nullptr, &nearstar_w, &nearstar_h);
        SDL_QueryTexture(tex_farstar, nullptr, nullptr, &farstar_w, &farstar_h);
        nearstar_w /= 4;
        nearstar_h /= 4;
        farstar_w /= 2;
        farstar_h /= 2;
    }
    static uint32_t last_tick_ms = SDL_GetTicks();
    uint32_t cur_tick_ms = SDL_GetTicks();
    double delta_time = static_cast<double>(cur_tick_ms - last_tick_ms) / 1000.0;
    last_tick_ms = cur_tick_ms;

    // 背景群星移动效果
    static int line_nearpos_y = -nearstar_h;
    static int line_farpos_y = -farstar_h;
    static double speed_near = 150, speed_far = 80;
    line_farpos_y = line_farpos_y + static_cast<int>(speed_far * delta_time);
    line_nearpos_y = line_nearpos_y + static_cast<int>(speed_near * delta_time);
    if(line_farpos_y > 0) line_farpos_y = -farstar_h;
    if(line_nearpos_y > 0) line_nearpos_y = -nearstar_h;
    // 重复铺开贴图
    SDL_Point point_near = {0, line_nearpos_y};
    SDL_Point point_far = {0, line_farpos_y};
    for(int x = point_near.x; x < window_w; x += nearstar_w)
    {
        for(int y = point_near.y; y < window_h; y += nearstar_h)
        {
            SDL_Rect rect =  {x, y, nearstar_w, nearstar_h};
            SDL_RenderCopy(renderer, tex_nearstar, nullptr, &rect);
        }
    }
    for(int x = point_far.x; x < window_w; x += farstar_w)
    {
        for(int y = point_far.y; y < window_h; y += farstar_h)
        {
            SDL_Rect rect =  {x, y, farstar_w, farstar_h};
            SDL_RenderCopy(renderer, tex_farstar, nullptr, &rect);
        }
    }
}

void GameMgr::loadGame()
{
    std::ifstream input("score_list.txt");
    if(input.fail()) return;
    int score = 0;
    std::string name;
    ranking_list.clear();
    while(input >> name >> score)
    {
        ranking_list.insert({score, name});
    }
    input.close();
}

void GameMgr::saveGame()
{
    std::ofstream output("score_list.txt", std::ios::trunc);
    if(!output.is_open()) return;
    for(auto& [score, name] : ranking_list)
    {
        output << name << "\t" << score << std::endl;
    }
    output.close();
}