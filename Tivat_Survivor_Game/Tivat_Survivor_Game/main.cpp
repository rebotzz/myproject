#include "game.hpp"


static const int WINDOW_WIDTH = 1270;
static const int WINDOW_HEIGHT = 720;

// ����֡����
int idx_current_anim = 0;



int main()
{
	// ��ʼ��
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

		// ������Ϣ: �첽
		if (peekmessage(&msg, EX_KEY | EX_MOUSE))
		{
			player.processEvent(msg);
		}

		// ��������
		tryGenerateEnmey(enemy_list);
		player.move();
		for (Enemy* enemy : enemy_list)
			enemy->move(player);

		// ��ȾͼƬ
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