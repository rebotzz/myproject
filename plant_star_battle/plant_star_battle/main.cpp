#include <memory>
#include <easyx.h>
#include "util.h"
#include "atlas.h"
#include "menu_scene.h"
#include "game_scene.h"
#include "selector_scene.h"
#include "scene_manager.h"
#include "platform.h"
#include "player.h"

// ֲ�����Ǵ��Ҷ�

// ��Ϸ��Դ
// ��ɫͷ��ͼ��
IMAGE img_avatar_gloomshroom_right;
IMAGE img_avatar_nut_right;
IMAGE img_avatar_peashooter_right;
IMAGE img_avatar_sunflower_right;

IMAGE img_avatar_gloomshroom_left;
IMAGE img_avatar_nut_left;
IMAGE img_avatar_peashooter_left;
IMAGE img_avatar_sunflower_left;

// ����buff
Atlas atlas_buff_box_blue;
Atlas atlas_buff_box_pick;
Atlas atlas_buff_box_yellow;

IMAGE img_buff_icon_hurry;
IMAGE img_buff_icon_invisible;
IMAGE img_buff_icon_recover;
IMAGE img_buff_icon_silence;

// ������Ч
Atlas atlas_jump_effect;
Atlas atlas_land_effect;
Atlas atlas_run_effect;


// ��ɫ����֡
// ����Ģ��
Atlas atlas_gloomshroom_bubbles;
Atlas atlas_gloomshroom_bubbles_ex;
Atlas atlas_player_gloomshroom_attack_ex_right;
Atlas atlas_player_gloomshroom_die_right;
Atlas atlas_player_gloomshroom_idle_right;
Atlas atlas_player_gloomshroom_run_right;

Atlas atlas_player_gloomshroom_attack_ex_left;
Atlas atlas_player_gloomshroom_die_left;
Atlas atlas_player_gloomshroom_idle_left;
Atlas atlas_player_gloomshroom_run_left;

// ��� 
Atlas atlas_player_nut_attack_ex_right;
Atlas atlas_player_nut_die_right;
Atlas atlas_player_nut_idle_right;
Atlas atlas_player_nut_run_right;
Atlas atlas_player_nut_explode_right;

Atlas atlas_player_nut_attack_ex_left;
Atlas atlas_player_nut_die_left;
Atlas atlas_player_nut_idle_left;
Atlas atlas_player_nut_run_left;
Atlas atlas_player_nut_explode_left;

// �㶹����
IMAGE img_pea_bullet;
Atlas atlas_pea_bullet_break;
Atlas atlas_player_peashooter_attack_ex_right;
Atlas atlas_player_peashooter_die_right;
Atlas atlas_player_peashooter_idle_right;
Atlas atlas_player_peashooter_run_right;

Atlas atlas_player_peashooter_attack_ex_left;
Atlas atlas_player_peashooter_die_left;
Atlas atlas_player_peashooter_idle_left;
Atlas atlas_player_peashooter_run_left;

// ���տ�
Atlas atlas_sunflower_sun;
Atlas atlas_sunflower_sun_explode;
Atlas atlas_sunflower_sun_ex;
Atlas atlas_sunflower_sun_ex_explode;
Atlas atlas_sunflower_sun_text;
Atlas atlas_player_sunflower_attack_ex_right;
Atlas atlas_player_sunflower_die_right;
Atlas atlas_player_sunflower_idle_right;
Atlas atlas_player_sunflower_run_right;

Atlas atlas_player_sunflower_attack_ex_left;
Atlas atlas_player_sunflower_die_left;
Atlas atlas_player_sunflower_idle_left;
Atlas atlas_player_sunflower_run_left;


// ��ұ�ʶ
IMAGE img_IP_cursor;
IMAGE img_1P_winner;
IMAGE img_2P_cursor;
IMAGE img_2P_winner;
IMAGE winner_bar;

// �˵�����
IMAGE img_menu_background;

// ��ɫѡ�����
IMAGE img_VS;
IMAGE img_selector_background;
IMAGE img_selector_tips;
IMAGE img_gravestone_right;
IMAGE img_gravestone_left;

IMAGE img_1P;
IMAGE img_1P_desc;
IMAGE img_1P_selector_btn_down_right;
IMAGE img_1P_selector_btn_idle_right;
IMAGE img_1P_selector_btn_down_left;
IMAGE img_1P_selector_btn_idle_left;

IMAGE img_2P;
IMAGE img_2P_desc;
IMAGE img_2P_selector_btn_down_right;
IMAGE img_2P_selector_btn_idle_right;
IMAGE img_2P_selector_btn_down_left;
IMAGE img_2P_selector_btn_idle_left;

IMAGE img_gloomshroom_selector_background_right;
IMAGE img_nut_selector_background_right;
IMAGE img_peashooter_selector_background_right;
IMAGE img_sunflower_selector_background_right;

IMAGE img_gloomshroom_selector_background_left;
IMAGE img_nut_selector_background_left;
IMAGE img_peashooter_selector_background_left;
IMAGE img_sunflower_selector_background_left;

// ��Ϸ����
IMAGE img_hills;
IMAGE img_large_platform;
IMAGE img_small_platform;
IMAGE img_sky;



// ��Ļ�ߴ�
const int WINDOW_WIDTH = 1270;
const int WINDOW_HEIGHT = 720;


bool running = true;										// ��Ϸ�Ƿ��������
bool is_debug = false;										// �Ƿ�������ģʽ
std::shared_ptr<Scene> menu_scene(new MenuScene);			// �˵�����
std::shared_ptr<Scene> selector_scene(new SelectorScene);	// ѡ���ɫ����
std::shared_ptr<Scene> game_scene(new GameScene);			// ��Ϸ����
SceneManager scene_manager;									// ����������
Camera main_camera;											// �����
std::vector<Platform> platform_list;						// ���վ��ƽ̨
std::shared_ptr<Player> player_1 = nullptr;					// ���1
std::shared_ptr<Player> player_2 = nullptr;					// ���2


void flip_atlas(Atlas* src, Atlas* dst)
{
	dst->clear();
	for (int i = 0; i < src->get_size(); ++i)
	{
		IMAGE img_flipped;
		flip_image(src->get_image(i), &img_flipped);
		dst->add_image(img_flipped);
	}
}
void load_game_resources();

int main()
{
	// ��ʼ��
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	BeginBatchDraw();
	settextstyle(28, 0, _T("IPix"));
	setbkmode(TRANSPARENT);
	ExMessage msg;
	const int FPS = 60;

	load_game_resources();
	scene_manager.set_current_scene(menu_scene.get());


	while (running)
	{
		DWORD begin_time = GetTickCount();
		// ��Ϣ����

		if (peekmessage(&msg, EX_MOUSE | EX_KEY))
		{
			scene_manager.on_input(msg);
		}

		// ���ݴ���
		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick_time = current_tick_time - last_tick_time;
		scene_manager.on_update(delta_tick_time);
		last_tick_time = current_tick_time;



		// ����ͼƬ
		cleardevice();

		scene_manager.on_draw(main_camera);

		FlushBatchDraw();

		// ��̬��ʱ,��֤֡��
		DWORD end_time = GetTickCount();
		DWORD delta_time_ms = end_time - begin_time;
		if (delta_time_ms < 1000 / FPS)
			Sleep(1000 / FPS - delta_time_ms);
	}

	closegraph();
	EndBatchDraw();
	return 0;
}


void load_game_resources()
{
	// ��������
	AddFontResourceEx(_T("resources/IPix.ttf"), FR_PRIVATE, nullptr);

	// ����ͼƬ
	loadimage(&img_1P, L"resources/1P.png");
	loadimage(&img_IP_cursor, L"resources/1P_cursor.png");
	loadimage(&img_1P_desc, L"resources/1P_desc.png");
	loadimage(&img_1P_selector_btn_down_right, L"resources/1P_selector_btn_down.png");
	loadimage(&img_1P_selector_btn_idle_right, L"resources/1P_selector_btn_idle.png");
	flip_image(&img_1P_selector_btn_down_right, &img_1P_selector_btn_down_left);
	flip_image(&img_1P_selector_btn_idle_right, &img_1P_selector_btn_idle_left);
	loadimage(&img_1P_winner, L"resources/1P_winner.png");

	loadimage(&img_2P, L"resources/2P.png");
	loadimage(&img_2P_cursor, L"resources/2P_cursor.png");
	loadimage(&img_2P_desc, L"resources/2P_desc.png");
	loadimage(&img_2P_selector_btn_down_right, L"resources/2P_selector_btn_down.png");
	loadimage(&img_2P_selector_btn_idle_right, L"resources/2P_selector_btn_idle.png");
	flip_image(&img_2P_selector_btn_down_right, &img_2P_selector_btn_down_left);
	flip_image(&img_2P_selector_btn_idle_right, &img_2P_selector_btn_idle_left);
	loadimage(&img_2P_winner, L"resources/2P_winner.png");

	// ��ɫͷ��ͼ��
	loadimage(&winner_bar, L"resources/winner_bar.png");
	loadimage(&img_avatar_gloomshroom_right, L"resources/avatar_gloomshroom.png");
	loadimage(&img_avatar_nut_right, L"resources/avatar_nut.png");
	loadimage(&img_avatar_peashooter_right, L"resources/avatar_peashooter.png");
	loadimage(&img_avatar_sunflower_right, L"resources/avatar_sunflower.png");
	flip_image(&img_avatar_gloomshroom_right, &img_avatar_gloomshroom_left);
	flip_image(&img_avatar_nut_right, &img_avatar_nut_left);
	flip_image(&img_avatar_peashooter_right, &img_avatar_peashooter_left);
	flip_image(&img_avatar_sunflower_right, &img_avatar_sunflower_left);

	// ����buff
	atlas_buff_box_blue.load_from_file(L"resources/buff_box_blue_%d.png", 4);
	atlas_buff_box_pick.load_from_file(L"resources/buff_box_pink_%d.png", 4);
	atlas_buff_box_yellow.load_from_file(L"resources/buff_box_yellow_%d.png", 4);
	loadimage(&img_buff_icon_hurry, L"resources/buff_icon_hurry.png");
	loadimage(&img_buff_icon_invisible, L"resources/buff_icon_invisible.png");
	loadimage(&img_buff_icon_recover, L"resources/buff_icon_recover.png");
	loadimage(&img_buff_icon_silence, L"resources/buff_icon_silence.png");

	// ������Ч
	atlas_jump_effect.load_from_file(L"resources/jump_effect_%d.png", 5);
	atlas_land_effect.load_from_file(L"resources/land_effect_%d.png", 2);
	atlas_run_effect.load_from_file(L"resources/run_effect_%d.png", 4);

	// ��ɫ����֡
	// ������
	atlas_gloomshroom_bubbles.load_from_file(L"resources/bubbles_%d.png", 7);
	atlas_gloomshroom_bubbles_ex.load_from_file(L"resources/bubbles_ex_%d.png", 7);
	atlas_player_gloomshroom_attack_ex_right.load_from_file(L"resources/gloomshroom_attack_ex_%d.png", 7);
	atlas_player_gloomshroom_die_right.load_from_file(L"resources/gloomshroom_die_%d.png", 3);
	atlas_player_gloomshroom_idle_right.load_from_file(L"resources/gloomshroom_idle_%d.png", 4);
	atlas_player_gloomshroom_run_right.load_from_file(L"resources/gloomshroom_run_%d.png", 3);
	flip_atlas(&atlas_player_gloomshroom_attack_ex_right, &atlas_player_gloomshroom_attack_ex_left);
	flip_atlas(&atlas_player_gloomshroom_die_right, &atlas_player_gloomshroom_die_left);
	flip_atlas(&atlas_player_gloomshroom_idle_right, &atlas_player_gloomshroom_idle_left);
	flip_atlas(&atlas_player_gloomshroom_run_right, &atlas_player_gloomshroom_run_left);

	// ��� 
	atlas_player_nut_attack_ex_right.load_from_file(L"resources/nut_attack_ex_%d.png", 5);
	atlas_player_nut_die_right.load_from_file(L"resources/nut_die_%d.png", 3);
	atlas_player_nut_idle_right.load_from_file(L"resources/nut_idle_%d.png", 3);
	atlas_player_nut_run_right.load_from_file(L"resources/nut_run_%d.png", 3);
	atlas_player_nut_explode_right.load_from_file(L"resources/nut_explode_%d.png", 5);
	flip_atlas(&atlas_player_nut_attack_ex_right, &atlas_player_nut_attack_ex_left);
	flip_atlas(&atlas_player_nut_die_right, &atlas_player_nut_die_left);
	flip_atlas(&atlas_player_nut_idle_right, &atlas_player_nut_idle_left);
	flip_atlas(&atlas_player_nut_explode_right, &atlas_player_nut_explode_left);


	// �㶹����
	loadimage(&img_pea_bullet, L"resources/pea.png");
	atlas_pea_bullet_break.load_from_file(L"resources/pea_break_%d.png", 3);
	atlas_player_peashooter_attack_ex_right.load_from_file(L"resources/peashooter_attack_ex_%d.png", 3);
	atlas_player_peashooter_die_right.load_from_file(L"resources/peashooter_die_%d.png", 4);
	atlas_player_peashooter_idle_right.load_from_file(L"resources/peashooter_idle_%d.png", 9);
	atlas_player_peashooter_run_right.load_from_file(L"resources/peashooter_run_%d.png", 5);
	flip_atlas(&atlas_player_peashooter_attack_ex_right, &atlas_player_peashooter_attack_ex_left);
	flip_atlas(&atlas_player_peashooter_die_right, &atlas_player_peashooter_die_left);
	flip_atlas(&atlas_player_peashooter_idle_right, &atlas_player_peashooter_idle_left);
	flip_atlas(&atlas_player_peashooter_run_right, &atlas_player_peashooter_run_left);


	// ���տ�
	atlas_sunflower_sun.load_from_file(L"resources/sun_%d.png", 5);
	atlas_sunflower_sun_explode.load_from_file(L"resources/sun_explode_%d.png", 5);
	atlas_sunflower_sun_ex.load_from_file(L"resources/sun_ex_%d.png", 5);
	atlas_sunflower_sun_ex_explode.load_from_file(L"resources/sun_ex_explode_%d.png", 5);
	atlas_sunflower_sun_text.load_from_file(L"resources/sun_text_%d.png", 6);
	atlas_player_sunflower_attack_ex_right.load_from_file(L"resources/sunflower_attack_ex_%d.png", 9);
	atlas_player_sunflower_die_right.load_from_file(L"resources/sunflower_die_%d.png", 2);
	atlas_player_sunflower_idle_right.load_from_file(L"resources/sunflower_idle_%d.png", 8);
	atlas_player_sunflower_run_right.load_from_file(L"resources/sunflower_run_%d.png", 5);
	flip_atlas(&atlas_player_sunflower_attack_ex_right, &atlas_player_sunflower_attack_ex_left);
	flip_atlas(&atlas_player_sunflower_die_right, &atlas_player_sunflower_die_left);
	flip_atlas(&atlas_player_sunflower_idle_right, &atlas_player_sunflower_idle_left);
	flip_atlas(&atlas_player_sunflower_run_right, &atlas_player_sunflower_run_left);

	// �˵�����
	loadimage(&img_menu_background, L"resources/menu_background.png");

	// ��ɫѡ�����
	loadimage(&img_VS, L"resources/VS.png");
	loadimage(&img_selector_background, L"resources/selector_background.png");
	loadimage(&img_selector_tips, L"resources/selector_tip.png");
	loadimage(&img_gravestone_right, L"resources/gravestone.png");
	loadimage(&img_gloomshroom_selector_background_right, L"resources/gloomshroom_selector_background.png");
	loadimage(&img_nut_selector_background_right, L"resources/nut_selector_background.png");
	loadimage(&img_peashooter_selector_background_right, L"resources/peashooter_selector_background.png");
	loadimage(&img_sunflower_selector_background_right, L"resources/sunflower_selector_background.png");
	flip_image(&img_gravestone_right, &img_gravestone_left);
	flip_image(&img_gloomshroom_selector_background_right, &img_gloomshroom_selector_background_left);
	flip_image(&img_nut_selector_background_right, &img_nut_selector_background_left);
	flip_image(&img_peashooter_selector_background_right, &img_peashooter_selector_background_left);
	flip_image(&img_sunflower_selector_background_right, &img_sunflower_selector_background_left);

	// ��Ϸ����
	loadimage(&img_hills, L"resources/hills.png");
	loadimage(&img_large_platform, L"resources/platform_large.png");
	loadimage(&img_small_platform, L"resources/platform_small.png");
	loadimage(&img_sky, L"resources/sky.png");


	// ������Ƶ
	mciSendString(L"open resources/bgm_game.mp3 alias bgm_game", nullptr, 0, nullptr);
	mciSendString(L"open resources/bgm_menu.mp3 alias bgm_menu", nullptr, 0, nullptr);

	mciSendString(L"open resources/bubbles_shot.mp3 alias bubbles_shot", nullptr, 0, nullptr);
	mciSendString(L"open resources/bubbles_shot_ex.mp3 alias bubbles_shot_ex", nullptr, 0, nullptr);
	mciSendString(L"open resources/nut_explode.mp3 alias nut_explode", nullptr, 0, nullptr);
	mciSendString(L"open resources/pea_break_1.mp3 alias pea_break_1", nullptr, 0, nullptr);
	mciSendString(L"open resources/pea_break_2.mp3 alias pea_break_2", nullptr, 0, nullptr);
	mciSendString(L"open resources/pea_break_3.mp3 alias pea_break_3", nullptr, 0, nullptr);
	mciSendString(L"open resources/pea_shoot_ex.mp3 alias pea_shoot_ex", nullptr, 0, nullptr);
	mciSendString(L"open resources/pea_shoot_1.mp3 alias pea_shoot_1", nullptr, 0, nullptr);
	mciSendString(L"open resources/pea_shoot_2.mp3 alias pea_shoot_2", nullptr, 0, nullptr);
	mciSendString(L"open resources/sun_explode.mp3 alias sun_explode", nullptr, 0, nullptr);
	mciSendString(L"open resources/sun_explode_ex.mp3 alias sun_explode_ex", nullptr, 0, nullptr);
	mciSendString(L"open resources/sun_text.mp3 alias sun_text", nullptr, 0, nullptr);

	mciSendString(L"open resources/nut_dash.wav alias nut_dash", nullptr, 0, nullptr);
	mciSendString(L"open resources/ui_confirm.wav alias ui_confirm", nullptr, 0, nullptr);
	mciSendString(L"open resources/ui_switch.wav alias ui_switch", nullptr, 0, nullptr);
	mciSendString(L"open resources/ui_win.wav alias ui_win", nullptr, 0, nullptr);
}