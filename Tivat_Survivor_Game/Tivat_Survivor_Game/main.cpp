#include <thread>
#include <atomic>
#include "game.hpp"


const int WINDOW_WIDTH = 1270;
const int WINDOW_HEIGHT = 720;

const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75;

bool is_started_game = false;
std::atomic<bool> running = true;
std::atomic<bool> play_hitvoice = false;





int main()
{
	// 初始化

	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	BeginBatchDraw();
	srand((unsigned int)time(nullptr));
	// 获得窗口句柄
	HWND hWnd = GetHWnd();
	// 使用 Windows API 修改窗口名称
	SetWindowText(hWnd, L"提瓦特幸存者");

	// 加载资源
	Atlas* atlas_player_left = new Atlas(L"resource/img/player_left_%d.png", 6);
	Atlas* atlas_player_right = new Atlas(L"resource/img/player_right_%d.png", 6);
	Atlas* atlas_enemy_left = new Atlas(L"resource/img/enemy_left_%d.png", 6);
	Atlas* atlas_enemy_right = new Atlas(L"resource/img/enemy_right_%d.png", 6);

	RECT region_btn_start_game, region_btn_quit_game;
	
	region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
	region_btn_start_game.top = 430;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;

	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 550;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;

	StartGameButton btn_start_game(region_btn_start_game, _T("resource/img/ui_start_idle.png"),
		_T("resource/img/ui_start_hovered.png"), _T("resource/img/ui_start_pushed.png"));
	QuitGameButton btn_quit_game(region_btn_quit_game, _T("resource/img/ui_quit_idle.png"),
		_T("resource/img/ui_quit_hovered.png"), _T("resource/img/ui_quit_pushed.png"));

	ExMessage msg;
	IMAGE img_backgraund;
	IMAGE img_menu;
	Player player(atlas_player_left, atlas_player_right, L"resource/img/shadow_player.png");
	std::vector<Enemy*> enemy_list;
	std::vector<Bullet> bullet_list(3);
	int score = 0;

	loadimage(&img_backgraund, L"resource/img/background.png");
	loadimage(&img_menu, L"resource/img/menu.png");

	while (running)
	{
		DWORD begin_time = ::GetTickCount();

		// 处理消息: 异步
		if (peekmessage(&msg, EX_KEY | EX_MOUSE))
		{
			if(is_started_game)
				player.processEvent(msg);
			else
			{
				btn_start_game.processEvent(msg);
				btn_quit_game.processEvent(msg);
			}
		}

		// 处理数据

		if (is_started_game)
		{
			tryGenerateEnmey(enemy_list, atlas_enemy_left, atlas_enemy_right);
			player.move();
			for (Enemy* enemy : enemy_list)
			{
				enemy->move(player);
			}
			updateBullets(bullet_list, player);

			// 玩家与敌人碰撞检测
			for (Enemy* enemy : enemy_list)
			{
				if (enemy->checkPlayerCollision(player)) {
					player.hurt(70);
					if (player.checkAlive() == false) {
						running = false;
						std::wstring s;
						if (score < 60) s += L"挑战失败, 菜!";
						else if (score < 120) s += L"哟,有点长进";
						else if (score < 200) s += L"呵,不过如此!";
						else s += L"嗯,还行";
						s += L"\n共计得分: ";
						s += std::to_wstring(score);
						MessageBox(GetHWnd(), s.c_str(), L"游戏结束", MB_OK);
						break;
					}
				}
				else
					player.updateStatus();
			}
			// 子弹与敌人碰撞检测
			for (Bullet& bullet : bullet_list)
			{
				for (Enemy* enemy : enemy_list)
				{
					if (enemy->checkBulletCollision(bullet)) {
						enemy->hurt();
						//mciSendString(L"play hit from 0", nullptr, 0, nullptr);
						play_hitvoice = true;
					}
					else
						enemy->updateStatus();
				}
			}

			// 删除生命值为零的敌人
			for (int i = 0; i < enemy_list.size(); )
			{
				if (!enemy_list[i]->checkAlive()) {
					Enemy* enemy = enemy_list[i];
					std::swap(enemy_list[i], enemy_list.back());
					enemy_list.pop_back();
					delete enemy;
					score++;
					player.incrementMP();
					continue;
				}
				++i;
			}
		}


		// 渲染图片
		cleardevice();

		if (is_started_game)
		{
			putimage(0, 0, &img_backgraund);
			player.draw(1000 / 144);
			for (Enemy* enemy : enemy_list)
				enemy->draw(1000 / 144);
			for (Bullet& bullet : bullet_list)
				bullet.draw();
			player.drawStatusLine();
			drawPlayerScore(score);
		}
		else
		{
			putimage(0, 0, &img_menu);
			btn_start_game.draw();
			btn_quit_game.draw();
		}


		FlushBatchDraw();


		// 帧率控制
		DWORD end_time = ::GetTickCount();
		DWORD delta_time = end_time - begin_time;
		if (delta_time < 1000 / 144) {
			Sleep(1000 / 144 - delta_time);
		}
	}

	delete atlas_player_left;
	delete atlas_player_right;
	delete atlas_enemy_left;
	delete atlas_enemy_right;

	EndBatchDraw();
	closegraph();
	return 0;
}