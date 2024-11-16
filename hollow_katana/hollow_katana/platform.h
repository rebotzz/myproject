#pragma once
#include "collision_manager.h"
#include "character_manager.h"
#include "player.h"

#include <iostream>


// ˮƽ������ײƽ̨��
class Platform
{
public:
	enum class Mode
	{
		Normal,		// ��ͨƽ̨
		Flash,		// ����ƽ̨�����������ֵ�ƽ̨����(������¥�ݣ���Ȼ�ᶶ��)
		Transmit	// ����ƽ̨�������ʹ��͵�ĳ��λ��(�ƺ�������������Թ�/����)
	};
private:
	CollisionBox* collision_box = nullptr;		// ��ײ����
	Mode mode = Mode::Normal;					// ƽ̨����
	Vector2 target_position;					// ����Ŀ�ĵ�
	COLORREF color = RGB(255, 255, 255);		// ��ɫ
	bool is_enabled = true;						// �Ƿ�����

public:
	// λ��,����ê��ģʽ
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
				// ����ƽ̨��ײ���: ����ʱ,y������	
				Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
				if (mode == Mode::Transmit)
				{
					player->set_position(target_position);
					return;
				}

				const Vector2& player_velocity = player->get_velocity();
				const Vector2& player_position = player->get_position();
				// ������"վ��ƽ̨"����ƫ�£���ʱ����ײ
				//CollisionBox* last_platform = nullptr;
				//if(last_platform == collision_box)		
				player->on_platform(true);	// debug:��ͬƽ̨��λ��ײ���¸�����·-> ��ʱ���������ٽ�ƽ̨�߶ȹ���
				//last_platform = collision_box;

				if (player_velocity.y >= 0)
				{
					// debug:ϸ�ڣ�Ϊ�˼�����ײ,��΢ƫ��
					float platform_top = collision_box->get_position().y - collision_box->get_size().y / 2 + 5.0f;	
					if (mode == Mode::Flash)
					{
						// ��ؼ����character��on_update,������������ƽ̨�߶�
						player->set_velocity({ player_velocity.x, 0.0f });
						player->set_position({ player_position.x, platform_top });
						player->set_platform_floor_y(platform_top);
					}
					else if (mode == Mode::Normal)
					{
						// ����:�����һ֡�ŵ׸���ƽ̨,����������������ֻ�ƽ̨
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
					// ����
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
