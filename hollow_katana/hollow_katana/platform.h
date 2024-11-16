#pragma once
#include "collision_manager.h"
#include "character_manager.h"
#include "player.h"

#include <iostream>


// 水平单向碰撞平台类
class Platform
{
public:
	enum class Mode
	{
		Normal,		// 普通平台
		Flash,		// 闪现平台：碰到就闪现到平台顶部(可以做楼梯，虽然会抖动)
		Transmit	// 传送平台：碰到就传送到某个位置(似乎可以用来设计迷宫/机关)
	};
private:
	CollisionBox* collision_box = nullptr;		// 碰撞箱体
	Mode mode = Mode::Normal;					// 平台类型
	Vector2 target_position;					// 传送目的地
	COLORREF color = RGB(255, 255, 255);		// 颜色
	bool is_enabled = true;						// 是否启用

public:
	// 位置,中心锚点模式
	Platform(const Vector2& position, const Vector2& shape)
	{
		collision_box = CollisionManager::instance()->create_collision_box();
		collision_box->set_enabled(true);
		collision_box->set_layer_src(CollisionLayer::Scenery);
		collision_box->set_layer_dst(CollisionLayer::None);
		collision_box->set_position(position);
		collision_box->set_size(shape);

		collision_box->set_on_collision([&]()
			{
				// 单向平台碰撞检测: 下落时,y轴向下	
				Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
				if (mode == Mode::Transmit)
				{
					player->set_position(target_position);
					return;
				}

				const Vector2& player_velocity = player->get_velocity();
				const Vector2& player_position = player->get_position();
				// 条件："站在平台"（略偏下）上时算碰撞
				//CollisionBox* last_platform = nullptr;
				//if(last_platform == collision_box)		
				player->on_platform(true);	// debug:不同平台错位碰撞导致浮空走路-> 临时方案避免临近平台高度过低
				//last_platform = collision_box;

				if (player_velocity.y >= 0)
				{
					// debug:细节，为了继续碰撞,略微偏下
					float platform_top = collision_box->get_position().y - collision_box->get_size().y / 2 + 5.0f;	
					if (mode == Mode::Flash)
					{
						// 落地检测在character的on_update,所以这里设置平台高度
						player->set_velocity({ player_velocity.x, 0.0f });
						player->set_position({ player_position.x, platform_top });
						player->set_platform_floor_y(platform_top);
					}
					else if (mode == Mode::Normal)
					{
						// 条件:玩家上一帧脚底高于平台,避免下落过程中闪现回平台
						if (player->get_prev_frame_pos_y() <= platform_top)
						{
							player->set_velocity({ player_velocity.x, 0.0f });
							player->set_position({ player_position.x, platform_top });
							player->set_platform_floor_y(platform_top);
						}
					}
				}
				else
				{
					// 起跳
					player->set_platform_floor_y();
				}
			});
	}
	~Platform()
	{
		CollisionManager::instance()->destroy_collision_box(collision_box);
	}

	void set_enabled(bool flag)
	{
		is_enabled = flag;
		collision_box->set_enabled(flag);
	}

	void on_render()
	{
		if (is_enabled)
		{
		setlinestyle(PS_SOLID, 5);
		setlinecolor(color);
		rectangle((int)(collision_box->get_position().x - collision_box->get_size().x / 2),
				  (int)(collision_box->get_position().y - collision_box->get_size().y / 2),
				  (int)(collision_box->get_position().x + collision_box->get_size().x / 2),
				  (int)(collision_box->get_position().y + collision_box->get_size().y / 2));
		}
	}

	void set_mode(Mode target)
	{
		mode = target;
		switch (mode)
		{
		case Mode::Normal: color = RGB(255, 255, 255); break;
		case Mode::Flash: color = RGB(15, 70, 240); break;
		case Mode::Transmit: color = RGB(255, 100, 255); break;
		}
	}

	void set_target_position(const Vector2& target)
	{
		target_position = target;
	}
};
