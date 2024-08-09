#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "platform.h"

extern bool is_debug;
extern SceneManager scene_manager;
extern std::vector<Platform> platforms;

// ÓÎÏ·±³¾°
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


	virtual void on_enter()
	{
		_pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		_pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;
		_pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		_pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		platforms.resize(4);
		Platform& large_platform = platforms[0];
		large_platform._img = &img_large_platform;
		large_platform._render_position.x = 122;
		large_platform._render_position.y = 455;
		large_platform._shape.y = large_platform._render_position.y + 60;
		// Åö×²¼ì²é±ßÔµÂÔÐ¡ÓÚÍ¼Æ¬
		large_platform._shape.left = large_platform._render_position.x + 30;
		large_platform._shape.right = large_platform._render_position.x + large_platform._img->getwidth() - 30;

		Platform& small_platform_1 = platforms[1];
		small_platform_1._img = &img_small_platform;
		small_platform_1._render_position.x = 175;
		small_platform_1._render_position.y = 360;
		small_platform_1._shape.y = small_platform_1._render_position.y + small_platform_1._img->getheight() / 2;
		small_platform_1._shape.left = small_platform_1._render_position.x + 40;
		small_platform_1._shape.right = small_platform_1._render_position.x + small_platform_1._img->getwidth() - 40;

		Platform& small_platform_2 = platforms[2];
		small_platform_2._img = &img_small_platform;
		small_platform_2._render_position.x = 855;
		small_platform_2._render_position.y = 360;
		small_platform_2._shape.y = small_platform_2._render_position.y + small_platform_2._img->getheight() / 2;
		small_platform_2._shape.left = small_platform_2._render_position.x + 40;
		small_platform_2._shape.right = small_platform_2._render_position.x + small_platform_2._img->getwidth() - 40;

		Platform& small_platform_3 = platforms[3];
		small_platform_3._img = &img_small_platform;
		small_platform_3._render_position.x = 515;
		small_platform_3._render_position.y = 225;
		small_platform_3._shape.y = small_platform_3._render_position.y + small_platform_3._img->getheight() / 2;
		small_platform_3._shape.left = small_platform_3._render_position.x + 40;
		small_platform_3._shape.right = small_platform_3._render_position.x + small_platform_3._img->getwidth() - 40;
	}
	virtual void on_input(const ExMessage& msg)
	{
		static const int VK_Q = 0x51;
		static const int VK_A = 0x41;
		static const int VK_W = 0x57;
		static const int VK_S = 0x53;
		static const int VK_D = 0x44;
		static const int VK_F = 0x46;
		static const int VK_G = 0x47;
		static const int VK_1 = 0x31;
		static const int VK_2 = 0x32;

		if (msg.message == WM_KEYDOWN)
		{

		}

		if (msg.message == WM_KEYUP)
		{
			switch (msg.vkcode)
			{
			case VK_Q:
				is_debug = !is_debug;
				break;
			case VK_A: break;
			case VK_W: break;
			case VK_S: break;
			case VK_F: break;
			case VK_G: break;

			case VK_LEFT: break;
			case VK_RIGHT: break;
			case VK_UP: break;
			case VK_1: break;
			case VK_2: break;
			}
		}
	}
	virtual void on_update(int delta_time)
	{

	}
	virtual void on_draw(const Camera& camera)
	{
		// ±³¾°
		putimage_alpha(_pos_img_sky.x, _pos_img_sky.y, &img_sky);
		putimage_alpha(_pos_img_hills.x, _pos_img_hills.y, &img_hills);

		// Æ½Ì¨
		for (Platform& platform : platforms)
			platform.on_draw(camera);

		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(10, 10, L"µ÷ÊÔÄ£Ê½, ¹Ø±ÕÇë°´ 'Q' ¼ü");
			// »æÖÆÅö×²¼ì²âÏß
			setlinecolor(RGB(255, 0, 0));
			for (Platform& platform : platforms)
				line(camera, platform._shape.left, platform._shape.y, platform._shape.right, platform._shape.y);
		}

	}
	virtual void on_exit()
	{

	}
};