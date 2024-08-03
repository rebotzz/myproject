#include <thread>
#include <atomic>
#include "game.hpp"


const int WINDOW_WIDTH = 1270;
const int WINDOW_HEIGHT = 720;



std::atomic<bool> is_started_game = false;
std::atomic<bool> running = true;
std::atomic<bool> play_hitvoice_enemy = false;
std::atomic<bool> play_hurtvoice_player = false;



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

	// 加载资源: Atlas共享资源,最上层管理
	std::shared_ptr<Atlas> atlas_player_left = nullptr, atlas_player_right = nullptr;
	std::shared_ptr<Atlas> atlas_player_left_sketch = nullptr, atlas_player_right_sketch = nullptr;
	CharactersAtlas::loadCharacterAtlas(L"resource/img/player_left_%d.png", 6, atlas_player_left, atlas_player_right, atlas_player_left_sketch, atlas_player_right_sketch);

	// 敌人
	std::vector<CharactersAtlas> enemys;
	enemys.push_back(CharactersAtlas(L"resource/img/bee_left_%d.png", 4));
	enemys.push_back(CharactersAtlas(L"resource/img/boar_left_%d.png", 6));
	enemys.push_back(CharactersAtlas(L"resource/img/snail_left_%d.png", 8));
	CharactersAtlas& enemy_bee = enemys[0];
	CharactersAtlas& enemy_boar = enemys[1];
	CharactersAtlas& enemy_snail = enemys[2];


	std::shared_ptr<IMAGE> img_player_shadow(new IMAGE), img_enemy_shadow(new IMAGE);
	loadimage(img_player_shadow.get(), L"resource/img/shadow_player.png");
	loadimage(img_enemy_shadow.get(), L"resource/img/shadow_enemy.png");
	
	std::shared_ptr<StartGameButton> btn_start_game = nullptr;
	std::shared_ptr<QuitGameButton> btn_quit_game = nullptr;
	loadButtonImage(btn_start_game, btn_quit_game);

	ExMessage msg;
	IMAGE img_backgraund;
	IMAGE img_menu;
	Player player(atlas_player_left.get(), atlas_player_right.get(), atlas_player_left_sketch.get(), atlas_player_right_sketch.get(), img_player_shadow.get());
	player.setCharaterAttribute(CharaterAttribute(3, 5, 300));
	std::vector<Enemy*> enemy_list;
	std::vector<Bullet> bullet_list;	// todo:增加非绕玩家旋转子弹
	int score = 0;

	loadimage(&img_backgraund, L"resource/img/background.png");
	loadimage(&img_menu, L"resource/img/menu.png");
	DWORD anim_delta_time = 0;

	while (running)
	{
		DWORD begin_time = ::GetTickCount();

		// 处理消息: 异步
		if (peekmessage(&msg, EX_KEY | EX_MOUSE))
		{
			if (is_started_game)
				player.processEvent(msg);
			else
			{
				btn_start_game->processEvent(msg);
				btn_quit_game->processEvent(msg);
			}
		}

		// 处理数据

		if (is_started_game)
		{
			// 蜜蜂敌人
			CharactersAtlas* enemy = nullptr;
			CharaterAttribute enemy_attri;
			int choice_enemy = rand() % 10;
			if (choice_enemy <= 2) {
				enemy_attri = CharaterAttribute(3, 1, 110);
				enemy = &enemy_bee;
			}
			// 野猪敌人
			else if (choice_enemy <= 7) {
				enemy_attri = CharaterAttribute(2, 2, 130);
				enemy = &enemy_boar;
			}
			//// 蜗牛敌人
			//else {
			//	enemy_attri = CharaterAttribute(2, 3, 150);
			//	enemy = &enemy_snail;
			//}
			tryGenerateEnmey(enemy_list, enemy->get_left(), enemy->get_right(), enemy->get_left_sketch(), enemy->get_right_sketch(), img_enemy_shadow.get(), enemy_attri);

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
					player.hurt();
					// 回到开始界面,重置敌人,玩家;
					if (player.checkAlive() == false) {
						is_started_game = false;
						for (Enemy* enemy : enemy_list)
							enemy->kill();
						player.reset();

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
			player.draw(anim_delta_time);
			//player.testRenderFrozen();

			for (Enemy* enemy : enemy_list)
				enemy->draw(anim_delta_time);
			for (Bullet& bullet : bullet_list)
				bullet.draw();
			player.drawStatusLine();
			drawPlayerScore(score);
		}
		else
		{
			putimage(0, 0, &img_menu);
			btn_start_game->draw();
			btn_quit_game->draw();
		}


		FlushBatchDraw();


		// 帧率控制
		DWORD end_time = ::GetTickCount();
		DWORD delta_time = end_time - begin_time;
		if (delta_time < 1000 / 144) 
			Sleep(1000 / 144 - delta_time);
		anim_delta_time = GetTickCount() - begin_time;
	}


	EndBatchDraw();
	closegraph();

	return 0;
}