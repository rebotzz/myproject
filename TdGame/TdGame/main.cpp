#define SDL_MAIN_HANDLED

#include "game_manager.h"

// 游戏开发是一门妥协的艺术

int main()
{
	return GameMgr::instance()->run();
}