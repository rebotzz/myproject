#include <chrono>
#include "game.hpp"
#include "scene.h"

// 扫雷格子数目
int grid_x = 16;
int grid_y = 16;

MenuScene menu_scene;
GameScene game_scene;
SceneManager scene_manager;

int main()
{
	// 初始化
	scene_manager.switch_scene(SceneManager::SceneType::Menu);
	const int FPS = 144;
	BeginBatchDraw();
	ExMessage msg;

	while (true)
	{
		std::chrono::nanoseconds begin_time;

		// 处理消息
		if (peekmessage(&msg, EX_MOUSE))
		{
			scene_manager.on_input(msg);
		}


		//scene_manager.on_update();


		cleardevice();

		scene_manager.on_draw();

		FlushBatchDraw();

		// 动态延时
	}

	closegraph();
	return 0;
}