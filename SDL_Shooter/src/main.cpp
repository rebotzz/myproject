#define SDL_MAIN_HANDLED
#include "gameMgr.h"

int main(int, char**)
{
    GameMgr::getInstance().run();
    return 0;
}