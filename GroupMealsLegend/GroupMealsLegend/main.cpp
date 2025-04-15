//#pragma comment (linker,"/subsystem:windows /entry:mainCRTStartup")		// 关闭控制台窗口

#include "game_system.h"
#undef main

// 即使代码支离破碎，也请您完成它。
int main()
{
	GameSystem::instance()->start();
}
