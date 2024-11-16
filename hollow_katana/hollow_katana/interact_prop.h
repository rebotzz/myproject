#pragma once
#include <string>
#include "collision_manager.h"
#include "util.h"

// 木头人
// 具有交互功能的道具
class InteractProp
{
private:
	CollisionBox* switch_box = nullptr;			// 开关碰撞箱体
	Vector2 position;							// 位置,中心锚点模式
	std::wstring desc;							// 描述文本
	CollisionBox* desc_box = nullptr;			// 描述碰撞箱体
	int desc_counter = 0;						// 关闭desc描述计数器
	bool is_showing_desc = false;				// 是否显示木头人描述

public:
	InteractProp()
	{
		switch_box = CollisionManager::instance()->create_collision_box();
		switch_box->set_enabled(true);
		switch_box->set_layer_src(CollisionLayer::Enemy);
		switch_box->set_layer_dst(CollisionLayer::None);

		desc_box = CollisionManager::instance()->create_collision_box();
		desc_box->set_enabled(true);
		desc_box->set_layer_src(CollisionLayer::Interact);
		desc_box->set_on_collision([&]()
			{
				is_showing_desc = true;
			});
	}
	~InteractProp()
	{
		CollisionManager::instance()->destroy_collision_box(switch_box);
		CollisionManager::instance()->destroy_collision_box(desc_box);
	}

	void on_render()
	{
		if (is_showing_desc)
		{
			settextcolor(RGB(255, 255, 255));
			outtextxy((int)position.x - textwidth(desc.c_str()) / 2,
				(int)(position.y - desc_box->get_size().y / 2) - textheight(desc.c_str()) - 10,
				desc.c_str());

			if (++desc_counter % 30 == 0)
				is_showing_desc = false;
		}
	}

	// 中心锚点模式
	void set_position(const Vector2& position)
	{
		this->position = position;
		switch_box->set_position(position);
		desc_box->set_position(position);
	}

	void set_size(const Vector2& size)
	{
		switch_box->set_size(size);
		desc_box->set_size(size);
	}
};

