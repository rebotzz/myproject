#define SDL_MAIN_HANDLED

#include "game_manager.h"

// ��Ϸ������һ����Э������

int main()
{
	return GameMgr::instance()->run();
}