#pragma comment (linker,"/subsystem:windows /entry:mainCRTStartup")		// �رտ���̨����

#include <memory>
#include <easyx.h>
#include <ctime>
#include "menu_scene.h"
#include "game_scene.h"
#include "selector_scene.h"
#include "scene_manager.h"
#include "player.h"

// ֲ�����Ǵ��Ҷ�

// ��Ϸ��Դ
// ��ɫͷ��ͼ��
IMAGE img_avatar_gloomshroom;
IMAGE img_avatar_nut;
IMAGE img_avatar_peashooter;
IMAGE img_avatar_sunflower;
IMAGE img_avatar_hornet;

// ����buff
Atlas atlas_buff_box_blue;
Atlas atlas_buff_box_grey;
Atlas atlas_buff_box_red;
Atlas atlas_buff_box_yellow;

IMAGE img_buff_icon_hurry;
IMAGE img_buff_icon_invisible;
IMAGE img_buff_icon_recover_hp;
IMAGE img_buff_icon_recover_mp;

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
Atlas atlas_nut_explode;

Atlas atlas_player_nut_attack_ex_left;
Atlas atlas_player_nut_die_left;
Atlas atlas_player_nut_idle_left;
Atlas atlas_player_nut_run_left;

// �㶹����
IMAGE img_pea_bullet;
Atlas atlas_pea_bullet_break;
Atlas atlas_player_peashoter_attack_ex_right;
Atlas atlas_player_peashoter_die_right;
Atlas atlas_player_peashoter_idle_right;
Atlas atlas_player_peashoter_run_right;

Atlas atlas_player_peashoter_attack_ex_left;
Atlas atlas_player_peashoter_die_left;
Atlas atlas_player_peashoter_idle_left;
Atlas atlas_player_peashoter_run_left;

// ���տ�
Atlas atlas_sunflower_sun_idle;
Atlas atlas_sunflower_sun_explode;
Atlas atlas_sunflower_sun_ex_idle;
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

// �ն���ʿ:��Ʒ�(С���)
Atlas atlas_player_hornet_idle_left;
Atlas atlas_player_hornet_idle_right;
Atlas atlas_player_hornet_run_left;
Atlas atlas_player_hornet_run_right;
Atlas atlas_player_hornet_jump_right;
Atlas atlas_player_hornet_jump_left;
Atlas atlas_player_hornet_fall_left; 
Atlas atlas_player_hornet_fall_right;

Atlas atlas_player_hornet_throw_silk_left;
Atlas atlas_player_hornet_throw_silk_right;
Atlas atlas_player_hornet_throw_sword_left;
Atlas atlas_player_hornet_throw_sword_right;
Atlas atlas_hornet_silk;
Atlas atlas_hornet_sword_left;
Atlas atlas_hornet_sword_right;

// ��ұ�ʶ
IMAGE img_1P_cursor;							// ���1ͷ�����
IMAGE img_2P_cursor;							// ���2ͷ�����
IMAGE img_1P_winner;							// ���1��ʤ������
IMAGE img_2P_winner;							// ���2��ʤ������
IMAGE img_winner_bar;							// ��ʤ��������ɫ

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
IMAGE img_hornet_selector_background_unkown;

// ��Ϸ����
IMAGE img_hills;
IMAGE img_large_platform;
IMAGE img_small_platform;
IMAGE img_sky;
IMAGE img_hollow_background;
IMAGE img_hollow_large_platform;
IMAGE img_hollow_small_platform;



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
std::shared_ptr<Player> player_1 = nullptr;					// ���1
std::shared_ptr<Player> player_2 = nullptr;					// ���2
IMAGE* img_player_1_avatar = nullptr;						// ���1ͷ��
IMAGE* img_player_2_avatar = nullptr;						// ���2ͷ��
std::vector<Platform> platform_list;						// ���վ��ƽ̨�б�
std::vector<Bullet*> bullet_list;							// ������ɵ��ӵ��б�
std::vector<Buff*> buff_list;								// �������buff�б�
GamePlace game_place = GamePlace::Nature;					// ��Ϸս������

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
	HWND hwnd = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetWindowText(hwnd, L"ֲ�����Ǵ��Ҷ�");
	BeginBatchDraw();
	settextstyle(28, 0, _T("IPix"));
	setbkmode(TRANSPARENT);
	ExMessage msg;
	const int FPS = 60;
	srand((unsigned int)time(nullptr));

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
	loadimage(&img_1P_cursor, L"resources/1P_cursor.png");
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
	loadimage(&img_winner_bar, L"resources/winner_bar.png");
	loadimage(&img_avatar_gloomshroom, L"resources/avatar_gloomshroom.png");
	loadimage(&img_avatar_nut, L"resources/avatar_nut.png");
	loadimage(&img_avatar_peashooter, L"resources/avatar_peashooter.png");
	loadimage(&img_avatar_sunflower, L"resources/avatar_sunflower.png");

	// ����buff
	atlas_buff_box_blue.load_from_file(L"resources/buff_box_blue_%d.png", 4);
	atlas_buff_box_red.load_from_file(L"resources/buff_box_red_%d.png", 4);
	atlas_buff_box_grey.load_from_file(L"resources/buff_box_grey_%d.png", 4);
	atlas_buff_box_yellow.load_from_file(L"resources/buff_box_yellow_%d.png", 4);
	loadimage(&img_buff_icon_hurry, L"resources/buff_icon_hurry.png");
	loadimage(&img_buff_icon_invisible, L"resources/buff_icon_invisible.png");
	loadimage(&img_buff_icon_recover_hp, L"resources/buff_icon_recover_hp.png");
	loadimage(&img_buff_icon_recover_mp, L"resources/buff_icon_recover_mp.png");

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
	atlas_nut_explode.load_from_file(L"resources/nut_explode_%d.png", 5);
	flip_atlas(&atlas_player_nut_attack_ex_right, &atlas_player_nut_attack_ex_left);
	flip_atlas(&atlas_player_nut_die_right, &atlas_player_nut_die_left);
	flip_atlas(&atlas_player_nut_idle_right, &atlas_player_nut_idle_left);
	flip_atlas(&atlas_player_nut_run_right, &atlas_player_nut_run_left);

	// �㶹����
	loadimage(&img_pea_bullet, L"resources/pea.png");
	atlas_pea_bullet_break.load_from_file(L"resources/pea_break_%d.png", 3);
	atlas_player_peashoter_attack_ex_right.load_from_file(L"resources/peashooter_attack_ex_%d.png", 3);
	atlas_player_peashoter_die_right.load_from_file(L"resources/peashooter_die_%d.png", 4);
	atlas_player_peashoter_idle_right.load_from_file(L"resources/peashooter_idle_%d.png", 9);
	atlas_player_peashoter_run_right.load_from_file(L"resources/peashooter_run_%d.png", 5);
	flip_atlas(&atlas_player_peashoter_attack_ex_right, &atlas_player_peashoter_attack_ex_left);
	flip_atlas(&atlas_player_peashoter_die_right, &atlas_player_peashoter_die_left);
	flip_atlas(&atlas_player_peashoter_idle_right, &atlas_player_peashoter_idle_left);
	flip_atlas(&atlas_player_peashoter_run_right, &atlas_player_peashoter_run_left);


	// ���տ�
	atlas_sunflower_sun_idle.load_from_file(L"resources/sun_%d.png", 5);
	atlas_sunflower_sun_explode.load_from_file(L"resources/sun_explode_%d.png", 5);
	atlas_sunflower_sun_ex_idle.load_from_file(L"resources/sun_ex_%d.png", 5);
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

	// �ն���ʿ:��Ʒ�(С���)
	loadimage(&img_avatar_hornet, L"resources/hornet/hornet_avatar.png");
	atlas_player_hornet_idle_left.load_from_file(L"resources/hornet/idle/%d.png", 6);
	flip_atlas(&atlas_player_hornet_idle_left, &atlas_player_hornet_idle_right);
	atlas_player_hornet_run_left.load_from_file(L"resources/hornet/run/%d.png", 8);
	flip_atlas(&atlas_player_hornet_run_left, &atlas_player_hornet_run_right);
	atlas_player_hornet_jump_left.load_from_file(L"resources/hornet/jump/%d.png", 8);
	flip_atlas(&atlas_player_hornet_jump_left, &atlas_player_hornet_jump_right);

	atlas_player_hornet_fall_left.load_from_file(L"resources/hornet/fall/%d.png", 4);
	flip_atlas(&atlas_player_hornet_fall_left, &atlas_player_hornet_fall_right);
	atlas_hornet_sword_left.load_from_file(L"resources/hornet/sword/%d.png", 3);
	flip_atlas(&atlas_hornet_sword_left, &atlas_hornet_sword_right);
	atlas_hornet_silk.load_from_file(L"resources/hornet/silk/%d.png", 9);
	atlas_player_hornet_throw_silk_left.load_from_file(L"resources/hornet/throw_silk/%d.png", 17);
	flip_atlas(&atlas_player_hornet_throw_silk_left, &atlas_player_hornet_throw_silk_right);
	atlas_player_hornet_throw_sword_left.load_from_file(L"resources/hornet/throw_sword/%d.png", 16);
	flip_atlas(&atlas_player_hornet_throw_sword_left, &atlas_player_hornet_throw_sword_right);

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
	loadimage(&img_hornet_selector_background_unkown, L"resources/hornet/hornet_selector_background.png");


	// ��Ϸ����
	loadimage(&img_hills, L"resources/hills.png");
	loadimage(&img_large_platform, L"resources/platform_large.png");
	loadimage(&img_small_platform, L"resources/platform_small.png");
	loadimage(&img_sky, L"resources/sky.png");
	loadimage(&img_hollow_background, L"resources/hollow/background.png");
	loadimage(&img_hollow_large_platform, L"resources/hollow/platform_large.png");
	loadimage(&img_hollow_small_platform, L"resources/hollow/platform_small.png");

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
	mciSendString(L"open resources/buff.mp3 alias buff", nullptr, 0, nullptr);

	mciSendString(L"open resources/nut_dash.wav alias nut_dash", nullptr, 0, nullptr);
	mciSendString(L"open resources/ui_confirm.wav alias ui_confirm", nullptr, 0, nullptr);
	mciSendString(L"open resources/ui_switch.wav alias ui_switch", nullptr, 0, nullptr);
	mciSendString(L"open resources/ui_win.wav alias ui_win", nullptr, 0, nullptr);

	mciSendString(L"open resources/hornet/audio/hurt_1.mp3 alias hornet_hurt_1", nullptr, 0, nullptr);
	mciSendString(L"open resources/hornet/audio/hurt_2.mp3 alias hornet_hurt_2", nullptr, 0, nullptr);
	mciSendString(L"open resources/hornet/audio/hurt_3.mp3 alias hornet_hurt_3", nullptr, 0, nullptr);
	mciSendString(L"open resources/hornet/audio/say_throw_silk_1.mp3 alias hornet_say_throw_silk_1", nullptr, 0, nullptr);
	mciSendString(L"open resources/hornet/audio/say_throw_silk_2.mp3 alias hornet_say_throw_silk_2", nullptr, 0, nullptr);
	mciSendString(L"open resources/hornet/audio/say_throw_sword.mp3 alias hornet_say_throw_sword", nullptr, 0, nullptr);
	mciSendString(L"open resources/hornet/audio/throw_silk.mp3 alias hornet_throw_silk", nullptr, 0, nullptr);
	mciSendString(L"open resources/hornet/audio/throw_sword.mp3 alias hornet_throw_sword", nullptr, 0, nullptr);
}