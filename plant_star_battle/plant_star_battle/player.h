#pragma once
#include <easyx.h>
#include "camera.h"
#include "vector2.h"
#include "animation.h"
#include "player_id.h"
#include "platform.h"


#include <iostream>
using std::cout;
using std::endl;

extern std::vector<Platform> platform_list;
extern bool is_debug;

class Player
{
protected:
	Animation _animation_idle_left;
	Animation _animation_idle_right;
	Animation _animation_run_left;
	Animation _animation_run_right;
	Animation* _current_animation = nullptr;	// ��ҵ�ǰ����
	PlayerID _id = PlayerID::P1;				// ������ID:���1,���2

	// ����ƶ�״̬
	bool _is_left_key_down = false;				// ����Ƿ������ƶ�
	bool _is_right_key_down = false;			// ����Ƿ������ƶ�
	bool _is_face_right = true;					// ���Ĭ�ϳ���
	Vector2 _position;							// ���λ��

	// ����ģ��
	const float _run_velocity = 0.55f;			// ���ˮƽ�ƶ��ٶ�
	const float _jump_velocity = -0.85f;		// ����������ٶ�
	Vector2 _velocity;							// ��ҵ�ǰ��ά�ٶ�ʸ��
	const float _gravity = 1.6e-3f;				// �������ٶ�: �Ի������Ϊ��
	Vector2 _size;								// �����ײ����
public:
	Player() = default;
	~Player() = default;

	void set_playerID(PlayerID id)
	{
		_id = id;
	}

	void set_position(const Vector2& position)
	{
		_position = position;
	}

	void on_input(const ExMessage& msg)
	{
		static const int VK_A = 0x41;
		static const int VK_W = 0x57;
		static const int VK_S = 0x53;
		static const int VK_D = 0x44;
		static const int VK_F = 0x46;
		static const int VK_G = 0x47;
		static const int VK_1 = 0x31;
		static const int VK_2 = 0x32;

		// ��ͬ���ID��Ӧ������ͬ
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (_id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case VK_A: _is_left_key_down = true; break;
				case VK_D: _is_right_key_down = true; break;
				case VK_W: on_jump();  break;
				case VK_F: break;
				case VK_G: break;
				}
				break;

			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT: _is_left_key_down = true; break;
				case VK_RIGHT: _is_right_key_down = true; break;
				case VK_UP: on_jump();  break;
				case VK_1: break;
				case VK_2: break;
				}
				break;
			}
			break;

		case WM_KEYUP:
			switch (_id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case VK_A: _is_left_key_down = false; break;
				case VK_D: _is_right_key_down = false; break;
				case VK_F: break;
				case VK_G: break;
				}
				break;

			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT: _is_left_key_down = false; break;
				case VK_RIGHT: _is_right_key_down = false; break;
				case VK_1: break;
				case VK_2: break;
				}
				break;
			}
			break;
		}
	}

	virtual void on_update(int interval_ms)
	{
		int direction = _is_right_key_down - _is_left_key_down;

		if (direction != 0)
		{
			_is_face_right = direction > 0;
			_current_animation = (_is_face_right ? &_animation_run_right : &_animation_run_left);
			float distance = direction * _run_velocity * interval_ms;
			on_run(distance);
		}
		else
		{
			_current_animation = (_is_face_right ? &_animation_idle_right : &_animation_idle_left);
		}

		move_and_collide(interval_ms);
		_current_animation->on_update(interval_ms);
	}

	virtual void on_draw(const Camera& camera)
	{
		_current_animation->on_draw(camera, (int)_position.x, (int)_position.y);

		if (is_debug)
		{
			setlinecolor(RGB(0, 255, 255));
			line(camera, (int)_position.x, (int)_position.y, (int)(_position.x + _size.x), (int)_position.y);
			line(camera, (int)_position.x, (int)_position.y, (int)_position.x, (int)(_position.y + _size.y));
			line(camera, (int)(_position.x + _size.x), (int)_position.y, (int)(_position.x + _size.x), (int)(_position.y + _size.y));
			line(camera, (int)_position.x, (int)(_position.y + _size.y), (int)(_position.x + _size.x), (int)(_position.y + _size.y));
		}
	}

protected:
	void on_jump()
	{
		// ����������,ֻ����ƽ̨��ʱ������
		if (_velocity.y != 0)
			return;

		_velocity.y = _jump_velocity;
	}

	void on_run(float distance)
	{
		_position.x += distance;
	}

	// ��ײ������ģ��
	void move_and_collide(int interval_ms)
	{
		_velocity.y += _gravity * (float)interval_ms;
		_position += _velocity * (float)interval_ms;

		// ����ƽ̨��ײ���: ����ʱ,y������
		if (_velocity.y > 0)
		{
			for (const Platform& platform : platform_list)
			{
				// x��,y����ײ���
				const Platform::CollisionShape& shape = platform._shape;
				bool is_x_collide = (max(_position.x + _size.x, shape.right) - min(_position.x, shape.left))
					< (_size.y + shape.right - shape.left);
				bool is_y_collide = (_position.y < shape.y && shape.y < _position.y + _size.y);

				if (is_x_collide && is_y_collide)
				{
					// ����:�����һ֡�ŵ׸���ƽ̨,����������������ֻ�ƽ̨
					float last_tick_pos_bottom_y = _position.y + _size.y - _velocity.y * interval_ms;
					if (last_tick_pos_bottom_y <= shape.y)
					{
						_velocity.y = 0;
						_position.y = shape.y - _size.y;
						break;
					}
				}
			}
		}
	}
};