#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "platform.h"
#include "player.h"									// debug:����include��Ӧͷ�ļ��������ֵı���
#include "status_bar.h"

extern bool is_debug;
extern SceneManager scene_manager;					
extern Camera main_camera;							// �����
extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;
extern std::shared_ptr<Player> player_1;			// ���1
extern std::shared_ptr<Player> player_2;			// ���2
extern IMAGE* img_player_1_avatar;						// ���1ͷ��
extern IMAGE* img_player_2_avatar;						// ���2ͷ��

// ��Ϸ����
extern IMAGE img_hills;
extern IMAGE img_large_platform;
extern IMAGE img_small_platform;
extern IMAGE img_sky;

class GameScene : public Scene
{
private:
	POINT _pos_img_hills = { 0, 0 };
	POINT _pos_img_sky = { 0, 0 };
	StatusBar _status_bar_1P;						// ���1״̬�� 
	StatusBar _status_bar_2P;						// ���2״̬�� 

public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter() override
	{
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

		player_1->set_position({ 200, 100 });
		player_2->set_position({ 900, 100 });

		_status_bar_1P.set_avatar(img_player_1_avatar);
		_status_bar_2P.set_avatar(img_player_2_avatar);
		_status_bar_1P.set_position(200, getheight() - 100);
		_status_bar_2P.set_position(750, getheight() - 100);

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

		// �Ƴ�����ɾ�����ӵ�: remove_if���ж�����Ϊ���Ԫ�ط���ĩβ,�������µ�ĩβ
		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(),
			[](Bullet* bullet)
			{
				bool deletable = bullet->check_can_remove();
				if (deletable) delete bullet;
				return deletable;
			}),
			bullet_list.end());

		// �����ӵ�״̬
		for (Bullet* bullet : bullet_list)
			bullet->on_update(interval_ms);

		// ����״̬��
		_status_bar_1P.set_hp(player_1->get_hp());
		_status_bar_1P.set_mp(player_1->get_mp());
		_status_bar_2P.set_hp(player_2->get_hp());
		_status_bar_2P.set_mp(player_2->get_mp());
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

		// �ӵ�
		for (Bullet* bullet : bullet_list)
			bullet->on_draw(camera);

		// ״̬��
		_status_bar_1P.on_draw();
		_status_bar_2P.on_draw();
	}

	virtual void on_exit()
	{
		mciSendString(L"stop bgm_game", nullptr, 0, nullptr);
	}
};