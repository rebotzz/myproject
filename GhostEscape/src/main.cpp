#include "core/game.h"
#include "glm/glm.hpp"


int main(int, char**)
{

    Game::getInstance().init("幽灵逃生 -by rebotzz", 1280, 720, 60);
    Game::getInstance().run();
    Game::getInstance().clean();

    // AssetStore::createFilenameResIDMap("assets");
    return 0;
}