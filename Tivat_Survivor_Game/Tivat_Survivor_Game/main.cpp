#include "game.hpp"


static const int WINDOW_WIDTH = 1270;
static const int WINDOW_HEIGHT = 720;

// 动画帧索引
int idx_current_anim = 0;



int main()
{
	// 初始化
	bool running = true;
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	BeginBatchDraw();
	srand(time(nullptr));

	ExMessage msg;
	IMAGE img_backgraund;
	Player player(6);
	std::vector<Enemy*> enemy_list;

	loadimage(&img_backgraund, L"resource/img/background.png");

	while (running)
	{
		DWORD begin_time = ::GetTickCount();

		// 处理消息: 异步
		if (peekmessage(&msg, EX_KEY | EX_MOUSE))
		{
			player.processEvent(msg);
		}

		// 处理数据
		tryGenerateEnmey(enemy_list);
		player.move();
		for (Enemy* enemy : enemy_list)
			enemy->move(player);

		// 渲染图片
		cleardevice();

		putimage(0, 0, &img_backgraund);
		player.draw(1000 / 144);
		for (Enemy* enemy : enemy_list)
			enemy->draw(1000 / 144);

		FlushBatchDraw();


		DWORD end_time = ::GetTickCount();
		DWORD delta_time = end_time - begin_time;
		if (delta_time < 1000 / 144) {
			Sleep(1000 / 144 - delta_time);
		}
	}



	EndBatchDraw();
	closegraph();
	return 0;
}