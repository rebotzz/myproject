#pragma once
#include "resources_manager.h"
#include "interact_prop.h"

// 木头人
class Woodenman
{
private:
	IMAGE* img = nullptr;						// 木头人贴图
	InteractProp prop;							// 木头人交互道具
	int showing_hit_counter = 0;				// 关闭hit描述计数器
	bool is_showing_hit = false;				// 是否显示hit描述

public:
	Woodenman()
		:prop(_T("测试木头人"))
	{
		img = ResourcesManager::instance()->find_image("woodenman");
		prop.set_on_interact([&](CollisionBox*)
			{
				is_showing_hit = true;
			});
		prop.set_size({ (float)img->getwidth() - 10, (float)img->getheight() - 20 });
	}
	~Woodenman()
	{}

	// 中心锚点模式
	void set_position(const Vector2& position)
	{
		prop.set_position(position);
	}

	Vector2 get_img_size() const
	{
		return { (float)img->getwidth(), (float)img->getheight() };;
	}

	void on_render()
	{
		putimage_alpha((int)prop.get_position().x - img->getwidth() / 2,
			(int)prop.get_position().y - img->getheight() / 2, img);

		prop.on_render();

		if (is_showing_hit)
		{
			settextcolor(RGB(255, 50, 50));
			outtextxy((int)prop.get_position().x - textwidth(_T("hp -1")) / 2,
				(int)prop.get_position().y - img->getheight() / 2 - textheight(_T("hp -1")) - 10,
				_T("hp -1"));

			if (++showing_hit_counter % 30 == 0)
				is_showing_hit = false;
		}
	}

	void set_enabled(bool flag)
	{
		prop.set_enabled(flag);
	}
};