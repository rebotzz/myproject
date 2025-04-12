//#pragma comment (linker,"/subsystem:windows /entry:mainCRTStartup")		// 关闭控制台窗口

#include "game_system.h"
#undef main

int main()
{
	GameSystem::instance()->start();
}
