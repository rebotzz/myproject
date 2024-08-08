#include "game.hpp"

const int WINDOW_WIDTH = 1270;
const int WINDOW_HEIGHT = 720;

std::atomic<bool> is_started_game = false;
std::atomic<bool> is_choice_player = false;
std::atomic<bool> running = true;
std::atomic<bool> play_hitvoice_enemy = false;
std::atomic<bool> play_hurtvoice_player = false;
std::atomic<int> player_choice = PAIMON;
std::thread* thread_music = nullptr;

int main()
{
	// ��ʼ��

	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	BeginBatchDraw();
	srand((unsigned int)time(nullptr));
	// ��ô��ھ��
	HWND hWnd = GetHWnd();
	// ʹ�� Windows API �޸Ĵ�������
	SetWindowText(hWnd, L"�������Ҵ���");

	// ������Դ: Atlas������Դ,���ϲ����
	// ���
	std::vector<CharactersAtlas> players_atlas;
	players_atlas.push_back(CharactersAtlas(L"resource/img/paimon_right_%d.png", 6));
	players_atlas.push_back(CharactersAtlas(L"resource/img/theresa_right_%d.png", 11));
	CharactersAtlas& player_atlas_paimon = players_atlas[0];
	CharactersAtlas& player_atlas_theresa = players_atlas[1];
	player_atlas_paimon.loadHeadImg(L"resource/img/avatar_paimon.png");
	player_atlas_theresa.loadHeadImg(L"resource/img/avatar_theresa.png");
	player_atlas_paimon.loadShadowImg(L"resource/img/shadow_player.png");
	player_atlas_theresa.atlas_character_shadow = player_atlas_paimon.atlas_character_shadow;


	// ����
	std::vector<CharactersAtlas> enemys_atlas;
	enemys_atlas.push_back(CharactersAtlas(L"resource/img/bee_right_%d.png", 4));
	enemys_atlas.push_back(CharactersAtlas(L"resource/img/boar_right_%d.png", 6));
	enemys_atlas.push_back(CharactersAtlas(L"resource/img/snail_right_%d.png", 8));
	enemys_atlas.push_back(CharactersAtlas(L"resource/img/worm_right_%d.png", 6));
	CharactersAtlas& enemy_atlas_bee = enemys_atlas[0];
	CharactersAtlas& enemy_atlas_boar = enemys_atlas[1];
	CharactersAtlas& enemy_atlas_snail = enemys_atlas[2];
	CharactersAtlas& enemy_atlas_worm = enemys_atlas[3];
	enemy_atlas_bee.loadShadowImg(L"resource/img/shadow_enemy.png");
	enemy_atlas_worm.atlas_character_shadow = enemy_atlas_snail.atlas_character_shadow
		= enemy_atlas_boar.atlas_character_shadow = enemy_atlas_bee.atlas_character_shadow;

	std::shared_ptr<StartGameButton> btn_start_game = nullptr;
	std::shared_ptr<QuitGameButton> btn_quit_game = nullptr;
	loadButtonImage(btn_start_game, btn_quit_game);

	ChoicePlayerButton btn_choice_player_paimon(RECT(), L"resource/img/avatar_paimon.png", L"resource/img/avatar_paimon.png", L"resource/img/avatar_paimon.png");
	ChoicePlayerButton btn_choice_player_theresa(RECT(), L"resource/img/avatar_theresa.png", L"resource/img/avatar_theresa.png", L"resource/img/avatar_theresa.png");
	btn_choice_player_paimon.setRegion(PAIMON, true);
	btn_choice_player_theresa.setRegion(THERESA, false);

	ExMessage msg;
	IMAGE img_backgraund;
	IMAGE img_menu;

	CharactersAtlas* cur_player = &player_atlas_paimon;
	Player player_paimon(cur_player->get_left(), cur_player->get_right(), cur_player->get_left_sketch(), cur_player->get_right_sketch(), cur_player->get_shadow(), cur_player->get_head());
	player_paimon.setCharaterAttribute(CharaterAttribute(40, 5, 1, 1000, 3));
	cur_player = &player_atlas_theresa;
	Player player_theresa(cur_player->get_left(), cur_player->get_right(), cur_player->get_left_sketch(), cur_player->get_right_sketch(), cur_player->get_shadow(), cur_player->get_head());
	player_theresa.setCharaterAttribute(CharaterAttribute(30, 5, 3, 1000, 6));
	Player* player = &player_paimon;

	std::vector<Enemy*> enemy_list;
	std::vector<Bullet> bullet_list;	// todo:���ӷ��������ת�ӵ�
	int score = 0;

	loadimage(&img_backgraund, L"resource/img/background.png");
	loadimage(&img_menu, L"resource/img/menu.png");
	DWORD anim_delta_time = 0;

	while (running)
	{
		// ���شӲ���ϵͳ�����������ĺ�����
		DWORD begin_time = ::GetTickCount();

		// ������Ϣ: �첽
		if (peekmessage(&msg, EX_KEY | EX_MOUSE))
		{
			if (is_started_game)
				player->processEvent(msg);
			else if (is_choice_player)
			{
				btn_choice_player_paimon.processEvent(msg);
				btn_choice_player_theresa.processEvent(msg);

				if (player_choice == PAIMON)
					player = &player_paimon;
				else if (player_choice == THERESA)
					player = &player_theresa;
			}
			else
			{
				btn_start_game->processEvent(msg);
				btn_quit_game->processEvent(msg);
			}
		}

		// ��������
		if (is_started_game)
		{
			CharactersAtlas* enemy = nullptr;
			CharaterAttribute enemy_attri;
			int choice_enemy = rand() % 10;
			// �۷����
			if (choice_enemy <= 1) {
				enemy_attri = CharaterAttribute(30, 1, 0, 300);
				enemy = &enemy_atlas_bee;
			}
			// Դʯ�����
			else if (choice_enemy <= 5) {
				enemy_attri = CharaterAttribute(18, 2, 0, 400);
				enemy = &enemy_atlas_worm;
			}
			// Ұ�����
			else if (choice_enemy <= 8) {
				enemy_attri = CharaterAttribute(22, 2, 0, 500);
				enemy = &enemy_atlas_boar;
			}
			// ��ţ����
			else if (choice_enemy == 9) {
				enemy_attri = CharaterAttribute(15, 3, 0, 600);
				enemy = &enemy_atlas_snail;
			}
			tryGenerateEnmey(enemy_list, enemy->get_left(), enemy->get_right(), enemy->get_left_sketch(), enemy->get_right_sketch(), enemy->get_shadow(), enemy_attri);

			player->move();
			player->updateStatus(anim_delta_time);
			for (Enemy* enemy : enemy_list)
			{
				enemy->updateStatus(anim_delta_time);
				enemy->move(*player);
			}
			updateBullets(bullet_list, *player);

			// ����������ײ���
			for (Enemy* enemy : enemy_list)
			{
				if (enemy->checkPlayerCollision(*player)) {
					player->hurt();
					// �ص���ʼ����,���õ���,���;
					if (player->checkAlive() == false) {
						std::wstring s;
						if (score < 120) s += L"��,������";
						else if (score < 240) s += L"Ӵ,�е㳤��";
						else if (score < 360) s += L"��,�ֽ�����";
						else if (score < 1000) s += L"��,����";
						else s += L"����,����.";
						s += L"\n���Ƶ÷�: ";
						s += std::to_wstring(score);
						MessageBox(GetHWnd(), s.c_str(), L"��Ϸ����", MB_OK);

						score = 0;
						is_started_game = false;
						is_choice_player = false;
						for (Enemy* enemy : enemy_list)
							enemy->kill();
						player->reset();
						break;
					}
				}
			}
			// �ӵ��������ײ���
			for (Bullet& bullet : bullet_list)
			{
				for (Enemy* enemy : enemy_list)
				{
					if (enemy->checkAlive() && enemy->checkBulletCollision(bullet)) {
						int damage_value = enemy->hurt();
						score += damage_value;
						player->incrementMP(damage_value);
					}
				}
			}

			// ɾ������ֵΪ��ĵ���
			for (int i = 0; i < enemy_list.size(); )
			{
				if (!enemy_list[i]->checkAlive()) {
					Enemy* enemy = enemy_list[i];
					std::swap(enemy_list[i], enemy_list.back());
					enemy_list.pop_back();
					delete enemy;
					if(is_started_game) {
						score++;
						player->incrementMP(1);
					}
					continue;
				}
				++i;
			}
		}


		// ��ȾͼƬ
		cleardevice();

		if (is_started_game)
		{
			putimage(0, 0, &img_backgraund);
			player->draw(anim_delta_time);
			for (Enemy* enemy : enemy_list)
				enemy->draw(anim_delta_time);
			for (Bullet& bullet : bullet_list)
				bullet.draw();
			player->drawStatusLine(score);
		}
		else if (is_choice_player)
		{
			putimage(0, 0, &img_backgraund);
			btn_choice_player_paimon.draw();
			btn_choice_player_theresa.draw();
		}
		else
		{
			putimage(0, 0, &img_menu);
			btn_start_game->draw();
			btn_quit_game->draw();
			setbkmode(TRANSPARENT);
			settextcolor(RGB(255, 128, 0));
			outtextxy(520, 690, L"Tip: ��ɫ�ƶ�[W] [A] [S] [D] or �����");
		}

		FlushBatchDraw();

		// ֡�ʿ���
		DWORD end_time = ::GetTickCount();
		DWORD delta_time = end_time - begin_time;
		if (delta_time < 1000 / 144)
			Sleep(1000 / 144 - delta_time);
		anim_delta_time = GetTickCount() - begin_time;
	}

	EndBatchDraw();
	closegraph();
	if (thread_music) {
		thread_music->join();
		delete thread_music;
	}

	return 0;
}