#include <iostream>
#include "core/game.h"
#include "glm/glm.hpp"


int main(int, char**)
{
    // glm::vec2 a = glm::vec2(0);
    // auto b = glm::normalize(a);
    // if(glm::length(b) > 0)
    //     SDL_Log("x:%f, y:%f", b.x, b.y);
    // else
    //     SDL_Log("vec2 is zero");


    Game::getInstance().init("幽灵逃生 -by rebotzz", 1280, 720, 60);
    Game::getInstance().run();
    Game::getInstance().clean();

    // AssetStore::createFilenameResIDMap("assets");
    return 0;
}