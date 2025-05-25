#include "core/game.h"

int main(int, char**)
{

    Game::getInstance().init("幽灵逃生 -by rebotzz", 1280, 720, 60);
    Game::getInstance().run();
    Game::getInstance().clean();

    // AssetStore::createFilenameResIDMap("assets");

    return 0;
}