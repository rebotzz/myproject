#include <chrono>
#include "game.hpp"
#include "scene.h"

// ɨ�׸�����Ŀ
int grid_x = 16;
int grid_y = 16;

MenuScene menu_scene;
GameScene game_scene;
SceneManager scene_manager;

int main()
{
	// ��ʼ��
	scene_manager.switch_scene(SceneManager::SceneType::Menu);
	const int FPS = 144;
	BeginBatchDraw();
	ExMessage msg;

	while (true)
	{
		std::chrono::nanoseconds begin_time;

		// ������Ϣ
		if (peekmessage(&msg, EX_MOUSE))
		{
			scene_manager.on_input(msg);
		}


		//scene_manager.on_update();


		cleardevice();

		scene_manager.on_draw();

		FlushBatchDraw();

		// ��̬��ʱ
	}

	closegraph();
	return 0;
}