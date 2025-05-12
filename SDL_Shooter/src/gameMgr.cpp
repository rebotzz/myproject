#include "gameMgr.h"
#include "resourcesMgr.h"
#include "sceneMain.h"

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

    return 0;
}

void GameMgr::switch_scene(Scene* scene)
{
    if(current_scene)
    {
        current_scene->exit();
        delete current_scene;
    }
    current_scene = scene;
    current_scene->enter();
}