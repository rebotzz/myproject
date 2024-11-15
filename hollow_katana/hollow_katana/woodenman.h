#pragma once
#include "collision_manager.h"
#include "resources_manager.h"
#include "util.h"

// 传送门
class Woodenman
{
private:
	IMAGE* img = nullptr;						// 木头人贴图
	CollisionBox* hurt_box = nullptr;			// 木头人碰撞箱体
	Vector2 position;							// 位置,中心锚点模式

	CollisionBox* describe_box = nullptr;		// 描述碰撞箱体
	int showing_desc_counter = 0;				// 关闭desc描述计数器
	bool is_showing_desc = false;				// 是否显示木头人描述
	int showing_hit_counter = 0;				// 关闭hit描述计数器
	bool is_showing_hit = false;				// 是否显示hit描述

public:
	Woodenman()
	{
		img = ResourcesManager::instance()->find_image("woodenman");
		hurt_box = CollisionManager::instance()->create_collision_box();
		hurt_box->set_enabled(true);
		hurt_box->set_layer_src(CollisionLayer::Enemy);
		hurt_box->set_layer_dst(CollisionLayer::None);
		hurt_box->set_on_collision([&]()
			{
				is_showing_hit = true;
			});
		hurt_box->set_size({ (float)img->getwidth() - 10, (float)img->getheight() - 20 });

		describe_box = CollisionManager::instance()->create_collision_box();
		describe_box->set_enabled(true);
		describe_box->set_layer_src(CollisionLayer::Scenery);
		describe_box->set_size({ (float)img->getwidth() - 10, (float)img->getheight() - 20 });
		describe_box->set_on_collision([&]()
			{
				is_showing_desc = true;
			});
	}
	~Woodenman()
	{
		CollisionManager::instance()->destroy_collision_box(hurt_box);
		CollisionManager::instance()->destroy_collision_box(describe_box);
	}

	// 中心锚点模式
	void set_position(const Vector2& position)
	{
		this->position = position;
		hurt_box->set_position(position);
		describe_box->set_position(position);
	}

	Vector2 get_img_size() const
	{
		return { (float)img->getwidth(), (float)img->getheight() };;
	}

	void on_render()
	{
		putimage_alpha((int)position.x - img->getwidth() / 2,
			(int)position.y - img->getheight() / 2, img);

		if (is_showing_desc)
		{
			settextcolor(RGB(255, 255, 255));
			outtextxy((int)position.x - textwidth(_T("测试木头人")) / 2,
				(int)position.y - img->getheight() / 2 - textheight(_T("测试木头人")) - 10,
				_T("测试木头人"));

			if (++showing_desc_counter % 30 == 0)
				is_showing_desc = false;
		}

		if (is_showing_hit)
		{
			settextcolor(RGB(255, 50, 50));
			outtextxy((int)position.x - textwidth(_T("hp -1")) / 2,
				(int)position.y - img->getheight() / 2 - textheight(_T("hp -1")) - 10,
				_T("hp -1"));

			if (++showing_hit_counter % 30 == 0)
				is_showing_hit = false;
		}

	}
};