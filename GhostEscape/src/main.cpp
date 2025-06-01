#include "core/game.h"
#include <iostream>

int main(int, char**)
{

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
    

    // AssetStore::createFilenameResIDMap("assets");

    return 0;
}