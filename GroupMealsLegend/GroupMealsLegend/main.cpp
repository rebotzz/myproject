//#pragma comment (linker,"/subsystem:windows /entry:mainCRTStartup")		// �رտ���̨����

#include "game_system.h"
#undef main

int main()
{
	GameSystem::instance()->start();
}
