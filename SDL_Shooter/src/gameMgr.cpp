#include "gameMgr.h"

GameMgr::GameMgr()
{
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
    if(!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL init failed: %s", SDL_GetError());
        exit(-1);
    }
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image init failed: %s", SDL_GetError());
        exit(-1);
    }
    if(Mix_Init(MIX_INIT_OGG | MIX_INIT_WAVPACK) != (MIX_INIT_OGG | MIX_INIT_WAVPACK)
    || !Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer init failed: %s", SDL_GetError());
        exit(-1);
    }
    Mix_AllocateChannels(32);
    if(!TTF_Init())
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

    // 主循环

    // 事件处理

    // 数据更新

    // 渲染纹理

    return 0;
}