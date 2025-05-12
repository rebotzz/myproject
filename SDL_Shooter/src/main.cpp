#define SDL_MAIN_HANDLED
#include <iostream>
#include "gameMgr.h"

int main(int, char**)
{
    SDL_Log("-----------test main.");

    try
    {
        GameMgr::getInstance().run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}