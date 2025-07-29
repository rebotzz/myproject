#include "core/game.h"
#include <iostream>

// 游戏开发是一门妥协的艺术。

// // 模板模式练习代码
// #define SDL_MAIN_USE_CALLBACKS 1
// #include <SDL3/SDL_main.h>
// // SDL启动时调用一次
// SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
// {
//     Game::getInstance().init("幽灵逃生 -by rebotzz", 1280, 720, 60);
//     return SDL_APP_CONTINUE;
// }

// // SDL循环中每帧调用：事件处理
// SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) 
// {
//     Game::getInstance().handleEvent(*event);
//     return SDL_APP_CONTINUE;
// }

// // SDL循环中每帧调用：游戏更新+渲染
// SDL_AppResult SDL_AppIterate(void *appstate)
// {
//     float frame_delta = Game::getInstance().controlFrameDelta();
//     Game::getInstance().update(frame_delta);
//     Game::getInstance().render();
//     if(Game::getInstance().getIsRunning()) return SDL_APP_CONTINUE;
//     else return SDL_APP_SUCCESS;
// }

// // SDL退出时调用一次
// void SDL_AppQuit(void *appstate, SDL_AppResult result)
// {
//     Game::getInstance().clean();
// }

int main(int, char**)
{
    // AssetStore::createFilenameResIDMap("assets");

    try
    {
        Game::getInstance().init("幽灵逃生 -by rebotzz", 1280, 720, 60);
        Game::getInstance().run();
        Game::getInstance().clean();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}