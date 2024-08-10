#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "platform.h"
#include "player.h"						// debug:忘记include对应头文件会出现奇怪的报错

extern bool is_debug;
extern SceneManager scene_manager;					
extern std::vector<Platform> platform_list;
extern std::shared_ptr<Player> player_1;			// 玩家1
extern std::shared_ptr<Player> player_2;			// 玩家2


// 游戏背景
extern IMAGE img_hills;
extern IMAGE img_large_platform;
extern IMAGE img_small_platform;
extern IMAGE img_sky;

class GameScene : public Scene
{
private:
	POINT _pos_img_hills = { 0, 0 };
	POINT _pos_img_sky = { 0, 0 };

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
		// 碰撞检查边缘略小于图片
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
		player_2->set_position({ 600, 100 });

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
		player_1->on_update(interval_ms);
		player_2->on_update(interval_ms);
	}

	virtual void on_draw(const Camera& camera) override
	{
		// 背景
		putimage_alpha(_pos_img_sky.x, _pos_img_sky.y, &img_sky);
		putimage_alpha(_pos_img_hills.x, _pos_img_hills.y, &img_hills);

		// 平台
		for (Platform& platform : platform_list)
			platform.on_draw(camera);

		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(10, 10, L"调试模式中, 按'Q'键关闭");
		}

		player_1->on_draw(camera);
		player_2->on_draw(camera);
	}

	virtual void on_exit()
	{
		mciSendString(L"stop bgm_game", nullptr, 0, nullptr);
	}
};