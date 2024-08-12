#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "platform.h"
#include "player.h"									// debug:����include��Ӧͷ�ļ��������ֵı���
#include "status_bar.h"
#include "buff_bullet.h"

extern bool is_debug;
extern SceneManager scene_manager;					
extern Camera main_camera;							// �����
extern std::shared_ptr<Player> player_1;			// ���1
extern std::shared_ptr<Player> player_2;			// ���2
extern IMAGE* img_player_1_avatar;					// ���1ͷ��
extern IMAGE* img_player_2_avatar;					// ���2ͷ��
extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;
extern std::vector<Buff*> buff_list;


// ��Ϸ����
extern IMAGE img_sky;								// �������
extern IMAGE img_hills;								// ����ɽ��
extern IMAGE img_large_platform;					// ���վ����ƽ̨
extern IMAGE img_small_platform;					// ���վ��Сƽ̨

// ��ұ�ʶ
extern IMAGE img_1P_cursor;							// ���1ͷ�����
extern IMAGE img_2P_cursor;							// ���2ͷ�����
extern IMAGE img_1P_winner;							// ���1��ʤ�ı�������
extern IMAGE img_2P_winner;							// ���2��ʤ�ı�������
extern IMAGE img_winner_bar;						// ��ʤ����������ͼ

class GameScene : public Scene
{
private:
	POINT _pos_img_hills = { 0, 0 };
	POINT _pos_img_sky = { 0, 0 };
	StatusBar _status_bar_1P;						// ���1״̬�� 
	StatusBar _status_bar_2P;						// ���2״̬�� 
	bool _is_showing_cursor = false;				// �Ƿ���ʾ���ͷ�����
	Timer _timer_showing_cursor;					// ��ʾ���ͷ����궨ʱ��

	// ��Ϸ��������
	bool _is_game_over = false;						// ��Ϸ�Ƿ����
	bool _is_slide_out_started = false;				// �Ƿ�ʼ������ʤ�ı�
	POINT _pos_img_winner_bar = { 0, 0 };			// ��һ�ʤ�����ı�λ��
	POINT _pos_img_winner_text = { 0, 0 };			// ��ʤ��������ɫλ��
	int _pos_x_img_winner_bar_dst = 0;				// ��������ͣλ��
	int _pos_x_img_winner_text_dst = 0;				// �����ı���ͣλ��
	Timer _timer_winner_slide_in;					// ���������붨ʱ��
	Timer _timer_winner_slide_out;					// ������������ʱ��
	float _speed_winner_bar = 1.95f;				// �����������ٶ�
	float _speed_winner_text = 1.65f;				// �����ı������ٶ�

	Timer _timer_random_buff;							// �������buff��ʱ��

public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter() override
	{
		_is_game_over = false;
		_is_slide_out_started = false;
		_is_showing_cursor = true;

		// ������ʼ��
		_pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		_pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;
		_pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		_pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		platform_list.resize(4);
		Platform& large_platform = platform_list[0];
		large_platform._img = &img_large_platform;
		large_platform._render_position.x = 122;
		large_platform._render_position.y = 455;
		large_platform._shape.y = (float)large_platform._render_position.y + 60;
		// ��ײ����Ե��С��ͼƬ
		large_platform._shape.left = (float)large_platform._render_position.x + 30;
		large_platform._shape.right = (float)large_platform._render_position.x + (float)large_platform._img->getwidth() - 30;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1._img = &img_small_platform;
		small_platform_1._render_position.x = 175;
		small_platform_1._render_position.y = 360;
		small_platform_1._shape.y = (float)small_platform_1._render_position.y + (float)small_platform_1._img->getheight() / 2;
		small_platform_1._shape.left = (float)small_platform_1._render_position.x + 40;
		small_platform_1._shape.right = (float)small_platform_1._render_position.x + (float)small_platform_1._img->getwidth() - 40;

		Platform& small_platform_2 = platform_list[2];
		small_platform_2._img = &img_small_platform;
		small_platform_2._render_position.x = 855;
		small_platform_2._render_position.y = 360;
		small_platform_2._shape.y = (float)small_platform_2._render_position.y + (float)small_platform_2._img->getheight() / 2;
		small_platform_2._shape.left = (float)small_platform_2._render_position.x + 40;
		small_platform_2._shape.right = (float)small_platform_2._render_position.x + (float)small_platform_2._img->getwidth() - 40;

		Platform& small_platform_3 = platform_list[3];
		small_platform_3._img = &img_small_platform;
		small_platform_3._render_position.x = 515;
		small_platform_3._render_position.y = 225;
		small_platform_3._shape.y = (float)small_platform_3._render_position.y + (float)small_platform_3._img->getheight() / 2;
		small_platform_3._shape.left = (float)small_platform_3._render_position.x + 40;
		small_platform_3._shape.right = (float)small_platform_3._render_position.x + (float)small_platform_3._img->getwidth() - 40;

		// ���λ��,�����ʼ��
		player_1->set_position({ 200, 50 });
		player_2->set_position({ 900, 50 });

		// ״̬����ʼ��
		_status_bar_1P.set_avatar(img_player_1_avatar);
		_status_bar_2P.set_avatar(img_player_2_avatar);
		_status_bar_1P.set_position(200, getheight() - 100);
		_status_bar_2P.set_position(750, getheight() - 100);

		// ��һ�ʤ����Ч����ʼ��
		_pos_img_winner_bar.x = 0 - img_winner_bar.getwidth();
		_pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		_pos_img_winner_text.x = 0 - img_1P_winner.getwidth();
		_pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		_pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;
		_pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;
		// ��ʤ����Ч���������2.5��,֮�󻬳�
		_timer_winner_slide_in.set_wait_time(2500);
		_timer_winner_slide_in.set_one_shot(true);
		_timer_winner_slide_in.set_callback([&]() {
			_is_slide_out_started = true;
			});
		// ��ʤ����Ч����������1��,֮�󳡾���ת�ز˵�
		_timer_winner_slide_out.set_wait_time(1000);
		_timer_winner_slide_out.set_one_shot(true);
		_timer_winner_slide_out.set_callback([]() {
			scene_manager.switch_scene(SceneManager::SceneType::MENU);
			});
		_timer_winner_slide_in.restart();
		_timer_winner_slide_out.restart();

		// ��ʾ���ͷ��1P,2P��궨ʱ����ʼ��
		_timer_showing_cursor.set_wait_time(2000);
		_timer_showing_cursor.set_one_shot(true);
		_timer_showing_cursor.set_callback([&]() { _is_showing_cursor = false; });
		_timer_showing_cursor.restart();

		// ����buff����
		_timer_random_buff.set_wait_time(7000);
		_timer_random_buff.set_one_shot(false);
		_timer_random_buff.set_callback(
			[&]()
			{
				int count = rand() % 2 + 1;
				for (int i = 0; i < count; ++i)
				{
					Buff* buff = nullptr;
					switch (rand() % 4)
					{
					case 0:
						buff = new BuffRecoveryHP;
						break;
					case 1:
						buff = new BuffRecoveryMP;
						break;
					case 2:
						buff = new BuffInvisible;
						break;
					case 3:
						buff = new BuffHurry;
						break;
					}
					buff_list.push_back(buff);
				}
			}
		);

		mciSendString(L"play bgm_game repeat from 0", nullptr, 0, nullptr);
	}
	virtual void on_input(const ExMessage& msg) override
	{
		static const int VK_Q = 0x51;

		player_1->on_input(msg);
		player_2->on_input(msg);

		if (msg.message == WM_KEYUP)
		{
			switch (msg.vkcode)
			{
			case VK_Q:
				is_debug = !is_debug;
				break;
			}
		}
	}

	virtual void on_update(int interval_ms) override
	{
		// �������
		player_1->on_update(interval_ms);
		player_2->on_update(interval_ms);

		// ���������
		main_camera.on_update(interval_ms);

		// ���³���buff
		_timer_random_buff.on_update(interval_ms);

		// �Ƴ�����ɾ�����ӵ�: remove_if���ж�����Ϊ���Ԫ�ط���ĩβ,�������µ�ĩβ
		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(),
			[](Bullet* bullet)
			{
				bool deletable = bullet->check_can_remove();
				if (deletable) delete bullet;
				return deletable;
			}),
			bullet_list.end());
		// �Ƴ�ʧЧbuff
		buff_list.erase(std::remove_if(buff_list.begin(), buff_list.end(),
			[](Bullet* buff)
			{
				bool deletable = buff->check_can_remove();
				if (deletable) delete buff;
				return deletable;
			}),
			buff_list.end());

		// �����ӵ�״̬
		for (Bullet* bullet : bullet_list)
			bullet->on_update(interval_ms);

		// ���³������buff״̬
		for (Bullet* buff : buff_list)
			buff->on_update(interval_ms);

		// ����״̬��
		_status_bar_1P.set_hp(player_1->get_hp());
		_status_bar_1P.set_mp(player_1->get_mp());
		_status_bar_1P.set_buff(player_1->get_buff());
		_status_bar_2P.set_hp(player_2->get_hp());
		_status_bar_2P.set_mp(player_2->get_mp());
		_status_bar_2P.set_buff(player_2->get_buff());

		// ������ʾ���1P,2P��굹��ʱ
		if (_is_showing_cursor)
			_timer_showing_cursor.on_update(interval_ms);

		// �������Ƿ�׹����Ļ��
		Vector2 position_1P = player_1->get_position();
		Vector2 position_2P = player_2->get_position();
		if (position_1P.y > getheight())
			player_1->set_hp(0);
		if (position_2P.y > getheight())
			player_2->set_hp(0);

		// ��Ϸ�������
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0)
		{
			if (!_is_game_over)
				mciSendString(L"play ui_win from 0", nullptr, 0, nullptr);

			_is_game_over = true;
		}

		if (_is_game_over)
		{
			_pos_img_winner_bar.x += (int)(_speed_winner_bar * interval_ms);
			_pos_img_winner_text.x += (int)(_speed_winner_text * interval_ms);

			// �ڻ�ʤ����Ч����ʼ����֮ǰ,winner��໬������Ļ�м�
			if (!_is_slide_out_started)
			{
				_timer_winner_slide_in.on_update(interval_ms);
				if (_pos_img_winner_bar.x >= _pos_x_img_winner_bar_dst)
					_pos_img_winner_bar.x = _pos_x_img_winner_bar_dst;
				if (_pos_img_winner_text.x >= _pos_x_img_winner_text_dst)
					_pos_img_winner_text.x = _pos_x_img_winner_text_dst;
			}
			else
				_timer_winner_slide_out.on_update(interval_ms);
		}
	}

	virtual void on_draw(const Camera& camera) override
	{
		// ����
		putimage_alpha(_pos_img_sky.x, _pos_img_sky.y, &img_sky);
		putimage_alpha(_pos_img_hills.x, _pos_img_hills.y, &img_hills);

		// ƽ̨
		for (Platform& platform : platform_list)
			platform.on_draw(camera);

		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(10, 10, L"����ģʽ��, ��'Q'���ر�");
		}

		// ���
		player_1->on_draw(camera);
		player_2->on_draw(camera);

		// ���ͷ��1P,2P���
		if (_is_showing_cursor)
		{
			Vector2 pos_1P_cursor, pos_2P_cursor;
			pos_1P_cursor.x = player_1->get_position().x + (player_1->get_size().x - img_1P_cursor.getwidth()) / 2;
			pos_1P_cursor.y = player_1->get_position().y + - img_1P_cursor.getheight();
			pos_2P_cursor.x = player_2->get_position().x + (player_2->get_size().x - img_2P_cursor.getwidth()) / 2;
			pos_2P_cursor.y = player_2->get_position().y + -img_1P_cursor.getheight();
			putimage_alpha(camera, (int)pos_1P_cursor.x, (int)pos_1P_cursor.y, &img_1P_cursor);
			putimage_alpha(camera, (int)pos_2P_cursor.x, (int)pos_2P_cursor.y, &img_2P_cursor);
		}

		// �ӵ�
		for (Bullet* bullet : bullet_list)
			bullet->on_draw(camera);

		// buff
		for (Bullet* buff : buff_list)
			buff->on_draw(camera);

		// ״̬��
		if (!_is_game_over)
		{
			_status_bar_1P.on_draw();
			_status_bar_2P.on_draw();
		}
		// ��Ϸ������һ�ʤ����Ч��
		else
		{
			putimage_alpha(camera, _pos_img_winner_bar.x, _pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(camera, _pos_img_winner_text.x, _pos_img_winner_text.y, 
				player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}

	}

	virtual void on_exit()
	{
		is_debug = false;
		_is_game_over = false;
		// ��Ҷ���ͨ������ָ���Զ��ͷ�(���ü���Ϊ0ʱ)

		for (Bullet* bullet : bullet_list)
			delete bullet;
		bullet_list.clear();

		for (Bullet* buff : buff_list)
			delete buff;
		buff_list.clear();

		mciSendString(L"stop bgm_game", nullptr, 0, nullptr);
	}
};