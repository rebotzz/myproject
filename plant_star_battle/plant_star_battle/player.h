#pragma once
#include <easyx.h>
#include "camera.h"
#include "animation.h"
#include "player_id.h"
#include "platform.h"
#include "bullet.h"
#include "particle.h"
#include "buff_id.h"
#include "buff_bullet.h"

extern bool is_debug;
extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;
extern std::vector<Buff*> buff_list;

// ������Ч
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;
extern Atlas atlas_run_effect;

class Player
{
public:
	// ���ê��λ��
	enum class AchorMode
	{
		Centered,
		BottomCentered,
		LeftTop			// Ϊ�˼���֮ǰ��
	};

protected:
	// ��Ҷ���
	Animation _animation_idle_left;				// ������ж�����
	Animation _animation_idle_right;			// ������ж�����
	Animation _animation_run_left;				// ��ұ��ܶ�����
	Animation _animation_run_right;				// ��ұ��ܶ�����
	Animation _animation_attack_ex_left;		// ������⹥��������
	Animation _animation_attack_ex_right;		// ������⹥��������
	Animation _animation_die_left;				// �������������
	Animation _animation_die_right;				// �������������
	Animation* _current_animation = nullptr;	// ��ҵ�ǰ����
	PlayerID _id = PlayerID::P1;				// ������ID:���1,���2
	IMAGE _img_sketch;							// ����ܵ�����ʱ��ɫ��Ӱ
	bool _is_update_animation = true;			// ����������Ҷ����ͼ����йص����

	// ����ƶ�״̬
	bool _is_left_key_down = false;				// ����Ƿ������ƶ�
	bool _is_right_key_down = false;			// ����Ƿ������ƶ�
	bool _is_face_right = true;					// ���Ĭ�ϳ���

	// ���״̬��
	int _hp = 100;								// ���Ѫ��
	int _mp = 0;								// ���������
	bool _can_attack = true;					// �ܷ���ͨ����
	bool _is_attack_ex = false;					// �Ƿ����ͷ����⹥��
	bool _is_invulnerable = false;				// ����Ƿ����޵�״̬
	bool _is_showing_sketch_frame = false;		// �Ƿ񲥷�����ܻ�ʱ��ɫ��Ӱ
	int _attack_cd = 500;						// ��ͨ����cd
	Timer _timer_attack_cd;						// ��ͨ������ʱ��
	Timer _timer_attack_ex;						// ���⹥��״̬��ʱ��
	Timer _timer_invulnerable;					// �޵�֡��ʱ��
	Timer _timer_invulnerable_blink;			// ����ܻ�ʱ��ɫ��Ӱ��˸���Ŷ�ʱ��
	Timer _timer_buff;							// ���buff����ʱ���ʱ��
	Timer _timer_buff_recovery;					// ��һָ�hp/mp��ʱ��
	BuffID _buff = BuffID::INVALID;				// ��ҵ�ǰbuff
	bool _is_invisible_player = false;			// ����Ƿ񲻿ɼ�

	// ����ģ��
	float _run_velocity = 0.55f;				// ���ˮƽ�ƶ��ٶ�
	float _jump_velocity = -0.85f;				// ����������ٶ�
	Vector2 _velocity;							// ��ҵ�ǰ��ά�ٶ�ʸ��
	float _gravity = 1.6e-3f;					// ����������ٶ�: �Ի������Ϊ��
	Vector2 _size;								// �����ײ����
	Vector2 _position;							// ���λ��
	AchorMode _achor_mode = AchorMode::LeftTop;	// �������λ�õ�ê��ģʽ

	// ��Ч
	std::vector<Particle> _particle_run_effect;	// �ܶ�������Ч
	std::vector<Particle> _particle_die_effect;	// ����������Ч
	Timer _timer_run_particle_effect_generation;// �ܶ�������Ч��ʱ��(�䵱���ӷ�����)
	Timer _timer_die_particle_effect_generation;// ����������Ч��ʱ��(�䵱���ӷ�����)
	Animation _animation_jump_effect;			// ��Ծ��Ч����
	Animation _animation_land_effect;			// �����Ч����
	bool _is_jump_effect_visible = false;		// ��Ծ��Ч�����Ƿ�ɼ�
	bool _is_land_effect_visible = false;		// �����Ч�����Ƿ�ɼ�
	Vector2 _position_jump_effect;				// ��Ծ��Ч����λ��
	Vector2 _position_land_effect;				// �����Ч����λ��
	Vector2 _last_hurt_direction;				// ���һ���е�����

public:
	Player(bool face_right = true) : _is_face_right(face_right)
	{
		_current_animation = _is_face_right ? &_animation_idle_right : &_animation_idle_left;

		_timer_attack_cd.set_one_shot(false);
		_timer_attack_cd.set_wait_time(_attack_cd);
		_timer_attack_cd.set_callback([&]() { _can_attack = true; });

		_timer_attack_ex.set_one_shot(true);
		_timer_attack_ex.set_wait_time(500);
		_timer_attack_ex.set_callback([&]() { _is_attack_ex = false; });

		_timer_invulnerable.set_wait_time(500); // �޵�֡ʱ��
		_timer_invulnerable.set_one_shot(true);
		_timer_invulnerable.set_callback([&]() { _is_invulnerable = false; });

		_timer_invulnerable_blink.set_wait_time(75);
		_timer_invulnerable_blink.set_one_shot(false);
		_timer_invulnerable_blink.set_callback([&]() { _is_showing_sketch_frame = !_is_showing_sketch_frame; });

		_timer_run_particle_effect_generation.set_wait_time(80);
		_timer_run_particle_effect_generation.set_one_shot(false);
		_timer_run_particle_effect_generation.set_callback(
			[&]()
			{
				// ����ҽ�����������
				Vector2 particle_position;
				IMAGE* effect_frame = atlas_run_effect.get_image(0);
				if (_achor_mode == AchorMode::LeftTop)
				{
					particle_position.x = _position.x + (_size.x - effect_frame->getwidth()) / 2;
					particle_position.y = _position.y + _size.y - effect_frame->getheight();
				}
				else if (_achor_mode == AchorMode::BottomCentered)
				{
					particle_position.x = _position.x - effect_frame->getwidth() / 2;
					particle_position.y = _position.y - effect_frame->getheight();
				}

				_particle_run_effect.emplace_back(&atlas_run_effect, particle_position, 75);
			}
		);

		_timer_die_particle_effect_generation.set_wait_time(50);
		_timer_die_particle_effect_generation.set_one_shot(false);
		_timer_die_particle_effect_generation.set_callback(
			[&]()
			{
				Vector2 particle_position;
				IMAGE* effect_frame = atlas_run_effect.get_image(0);
				if (_achor_mode == AchorMode::LeftTop)
				{
					particle_position.x = _position.x + (_size.x - effect_frame->getwidth()) / 2;
					particle_position.y = _position.y + _size.y - effect_frame->getheight();
				}
				else if (_achor_mode == AchorMode::BottomCentered)
				{
					particle_position.x = _position.x - effect_frame->getwidth() / 2;
					particle_position.y = _position.y - effect_frame->getheight();
				}

				_particle_die_effect.emplace_back(&atlas_run_effect, particle_position, 150);
			}
		);

		_timer_buff.set_wait_time(4000);
		_timer_buff.set_one_shot(true);
		_timer_buff.set_callback([&]()
			{
				// �ָ����ԭ��״̬,ȥ����ʱ״̬
				_buff = BuffID::INVALID;
				_is_invisible_player = false;
				_run_velocity = 0.55f;
			});
		_timer_buff_recovery.set_wait_time(300);
		_timer_buff_recovery.set_one_shot(false);
		_timer_buff_recovery.set_callback([&]()
			{
				if (_buff == BuffID::RECOVERY_HP)
					_hp += 2;
				else if (_buff == BuffID::RECOVERY_MP)
					_mp += 2;

				if (_mp > 100)
					_mp = 100;
				if (_hp > 100)
					_hp = 100;
			});

		_animation_jump_effect.set_atlas(&atlas_jump_effect);
		_animation_land_effect.set_atlas(&atlas_land_effect);
		_animation_jump_effect.set_interval(75);
		_animation_land_effect.set_interval(75);
		_animation_jump_effect.set_loop(false);
		_animation_land_effect.set_loop(false);
		_animation_jump_effect.set_callback([&]() { _is_jump_effect_visible = false; });
		_animation_land_effect.set_callback([&]() { _is_land_effect_visible = false; });
	}

	~Player() = default;

	void on_input(const ExMessage& msg)
	{
		static const int VK_A = 0x41;
		static const int VK_W = 0x57;
		static const int VK_S = 0x53;
		static const int VK_D = 0x44;
		static const int VK_F = 0x46;
		static const int VK_G = 0x47;

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
				case VK_F:
					if (_can_attack)
					{
						on_attack();
						_can_attack = false;
						_timer_attack_cd.restart();
					}
					break;
				case VK_G:
					if (_mp >= 100)
					{
						_mp = 0;
						on_attack_ex();
						_is_attack_ex = true;
						_timer_attack_ex.restart();
					}
					break;
				}
				break;

			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT: _is_left_key_down = true; break;
				case VK_RIGHT: _is_right_key_down = true; break;
				case VK_UP: on_jump();  break;
				case VK_NUMPAD1:
					if (_can_attack)
					{
						on_attack();
						_can_attack = false;
						_timer_attack_cd.restart();
					}
					break;
				case VK_NUMPAD2:
					if (_mp >= 100)
					{
						_mp = 0;
						on_attack_ex();
						_is_attack_ex = true;
						_timer_attack_ex.restart();
					}
					break;
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
				}
				break;

			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT: _is_left_key_down = false; break;
				case VK_RIGHT: _is_right_key_down = false; break;
				}
				break;
			}
			break;
		}
	}

	virtual void on_update(int interval_ms)
	{
		// ��ɫλ�ø���
		int direction = _is_right_key_down - _is_left_key_down;
		if (direction != 0 && _hp > 0)
		{
			if (!_is_attack_ex)
				_is_face_right = direction > 0;
			_current_animation = (_is_face_right ? &_animation_run_right : &_animation_run_left);
			float distance = direction * _run_velocity * interval_ms;
			on_run(distance);
		}
		else
		{
			_current_animation = (_is_face_right ? &_animation_idle_right : &_animation_idle_left);
			_timer_run_particle_effect_generation.pause();
		}
		// ���ұ߽�
		if (_position.x < (_achor_mode == AchorMode::LeftTop ? 0 : _size.x / 2))
			_position.x = (_achor_mode == AchorMode::LeftTop ? 0 : _size.x / 2);
		else if (_position.x > (getwidth() - (_achor_mode == AchorMode::LeftTop ? _size.x : _size.x / 2)))
			_position.x = (getwidth() - (_achor_mode == AchorMode::LeftTop ? _size.x : _size.x / 2));

		// �����ɫ����
		if (_hp <= 0)
			_current_animation = _last_hurt_direction.x > 0 ? &_animation_die_left : &_animation_die_right;

		// ���¼�ʱ��
		if (_is_attack_ex)
		{
			_current_animation = _is_face_right ? &_animation_attack_ex_right : &_animation_attack_ex_left;
			_timer_attack_ex.on_update(interval_ms);
		}

		if (_is_invulnerable)
		{
			_timer_invulnerable.on_update(interval_ms);
			_timer_invulnerable_blink.on_update(interval_ms);
			if (_is_showing_sketch_frame)
				sketch_image(_current_animation->get_frame(), &_img_sketch);
		}
		_timer_attack_cd.on_update(interval_ms);
		_timer_run_particle_effect_generation.on_update(interval_ms);
		if (_hp <= 0)
			_timer_die_particle_effect_generation.on_update(interval_ms);

		// ���µ�ǰ����
		if(_is_update_animation)
			_current_animation->on_update(interval_ms);

		// �����ж����
		move_and_collide(interval_ms);

		// ������Ч����
		if (_is_jump_effect_visible)
			_animation_jump_effect.on_update(interval_ms);
		if (_is_land_effect_visible)
			_animation_land_effect.on_update(interval_ms);

		// ɾ����Ч����
		_particle_run_effect.erase(std::remove_if(
			_particle_run_effect.begin(), _particle_run_effect.end(),
			[](const Particle& particle)
			{
				return !particle.check_valid();
			}),
			_particle_run_effect.end());

		_particle_die_effect.erase(std::remove_if(
			_particle_die_effect.begin(), _particle_die_effect.end(),
			[](const Particle& particle)
			{
				return !particle.check_valid();
			}),
			_particle_die_effect.end());

		// ��������״̬
		for (Particle& particle : _particle_run_effect)
			particle.on_update(interval_ms);
		for (Particle& particle : _particle_die_effect)
			particle.on_update(interval_ms);

		// buff
		if (_buff != BuffID::INVALID)
		{
			_timer_buff.on_update(interval_ms);
			_timer_buff_recovery.on_update(interval_ms);
		}
	}

	virtual void on_draw(const Camera& camera) const
	{
		// ������Ч
		for (const Particle& particle : _particle_run_effect)
			if (particle.check_valid())
				particle.on_draw(camera);

		for (const Particle& particle : _particle_die_effect)
			if (particle.check_valid())
				particle.on_draw(camera);

		// ��Ծ/�����Ч
		if (_is_jump_effect_visible)
			_animation_jump_effect.on_draw(camera, (int)_position_jump_effect.x, (int)_position_jump_effect.y);
		if (_is_land_effect_visible)
			_animation_land_effect.on_draw(camera, (int)_position_land_effect.x, (int)_position_land_effect.y);


		// ��Ҷ���
		Vector2 position_draw;
		IMAGE* frame = _current_animation->get_frame();
		if (_achor_mode == AchorMode::LeftTop)
			position_draw = _position;
		else if (_achor_mode == AchorMode::BottomCentered)
			position_draw = { _position.x - frame->getwidth() / 2, _position.y - frame->getheight() };

		if (!_is_invisible_player)
		{
			if (_hp > 0 && _is_invulnerable && _is_showing_sketch_frame)
				putimage_alpha(camera, (int)position_draw.x, (int)position_draw.y, (IMAGE*)&_img_sketch);
			else
				_current_animation->on_draw(camera, (int)position_draw.x, (int)position_draw.y);
		}

		if (is_debug)
		{
			Vector2 position_collision;
			if (_achor_mode == AchorMode::LeftTop)
				position_collision = _position;
			else if (_achor_mode == AchorMode::BottomCentered)
				position_collision = { _position.x - _size.x / 2, _position.y - _size.y };

			// �����ײ�߿�
			setlinecolor(RGB(0, 255, 255));
			rectangle(camera, (int)position_collision.x, (int)position_collision.y,
				(int)(position_collision.x + _size.x), (int)(position_collision.y + _size.y));

			// ��Ҷ���֡�߿�
			setlinecolor(RGB(0, 250, 154));
			rectangle(camera, (int)position_draw.x, (int)position_draw.y,
				(int)(position_draw.x + frame->getwidth()), (int)(position_draw.y + frame->getheight()));

			// ���λ��
			setfillcolor(RGB(255, 165, 0));
			solidcircle((int)_position.x, (int)_position.y, 5);
		}
	}


	void set_playerID(PlayerID id)
	{
		_id = id;
	}

	void set_position(const Vector2& position)
	{
		_position = position;
	}

	const Vector2& get_position() const
	{
		return _position;
	}

	const Vector2& get_size() const
	{
		return _size;
	}

	void set_hp(int val)
	{
		_hp = val;
	}

	int get_hp() const
	{
		return _hp;
	}

	int get_mp() const
	{
		return _mp;
	}

	BuffID get_buff()
	{
		return _buff;
	}

	AchorMode get_achor_mode()
	{
		return _achor_mode;
	}

protected:
	virtual void on_jump()
	{
		// ����������,ֻ����ƽ̨��ʱ������
		if (_velocity.y != 0 || _is_attack_ex)
			return;

		_velocity.y = _jump_velocity;

		_is_jump_effect_visible = true;
		_animation_jump_effect.reset();
		IMAGE* effect_frame = _animation_jump_effect.get_frame();

		if (_achor_mode == AchorMode::LeftTop)
		{
			_position_jump_effect.x = _position.x + (_size.x - effect_frame->getwidth()) / 2;
			_position_jump_effect.y = _position.y + _size.y - effect_frame->getheight();
		}
		else if (_achor_mode == AchorMode::BottomCentered)
		{
			_position_jump_effect.x = _position.x - effect_frame->getwidth() / 2;
			_position_jump_effect.y = _position.y - effect_frame->getheight();
		}
	}

	virtual void on_land()
	{
		_is_land_effect_visible = true;
		_animation_land_effect.reset();
		IMAGE* effect_frame = _animation_land_effect.get_frame();

		if (_achor_mode == AchorMode::LeftTop)
		{
			_position_land_effect.x = _position.x + (_size.x - effect_frame->getwidth()) / 2;
			_position_land_effect.y = _position.y + _size.y - effect_frame->getheight();
		}
		else if (_achor_mode == AchorMode::BottomCentered)
		{
			_position_land_effect.x = _position.x - effect_frame->getwidth() / 2;
			_position_land_effect.y = _position.y - effect_frame->getheight();
		}

	}

	void on_run(float distance)
	{
		if (_is_attack_ex)
			return;

		_position.x += distance;

		_timer_run_particle_effect_generation.resume();
	}

	// ��ײ������ģ��		
	void move_and_collide(int interval_ms)
	{
		float last_tick_velocity_y = _velocity.y;
		_velocity.y += _gravity * (float)interval_ms;
		_position += _velocity * (float)interval_ms;

		if (_hp <= 0)
			return;

		// ����ƽ̨��ײ���: ����ʱ,y������		bug: ��ҳ�ʼ������Ϸʱ����и��ʵ���ƽ̨��, float����? �ٶȹ������������?
		if (_velocity.y >= 0)
		{
			for (const Platform& platform : platform_list)
			{
				// x��,y����ײ���
				const Platform::CollisionShape& shape = platform._shape;
				bool is_x_collide = false, is_y_collide = false;
				if (_achor_mode == AchorMode::LeftTop)
				{
					is_x_collide = (max(_position.x + _size.x, shape.right) - min(_position.x, shape.left))
						< (_size.y + shape.right - shape.left);
					is_y_collide = (_position.y < shape.y && shape.y < _position.y + _size.y);
				}
				else if (_achor_mode == AchorMode::BottomCentered)
				{
					is_x_collide = (max(_position.x + _size.x / 2, shape.right) - min(_position.x - _size.x / 2, shape.left))
						< (_size.x + shape.right - shape.left);
					is_y_collide = (_position.y - _size.y < shape.y && shape.y < _position.y);
				}

				if (is_x_collide && is_y_collide)
				{
					// ����:�����һ֡�ŵ׸���ƽ̨,����������������ֻ�ƽ̨
					float last_tick_pos_bottom_y = 0;
					if (_achor_mode == AchorMode::LeftTop)
						last_tick_pos_bottom_y = _position.y + _size.y - _velocity.y * interval_ms;
					else if (_achor_mode == AchorMode::BottomCentered)
						last_tick_pos_bottom_y = _position.y - _velocity.y * interval_ms;

					static const float delta_y = 0.0001f;				// ��������
					if (last_tick_pos_bottom_y <= shape.y + delta_y)
					{
						_velocity.y = 0;
						if (_achor_mode == AchorMode::LeftTop)
							_position.y = shape.y - _size.y;
						else if (_achor_mode == AchorMode::BottomCentered)
							_position.y = shape.y;

						if (last_tick_velocity_y != 0)
							on_land();

						break;
					}
				}
			}
		}

		// �ӵ���ײ���
		if (!_is_invulnerable)
		{
			for (Bullet* bullet : bullet_list)
			{
				Vector2 position_collision;
				if (_achor_mode == AchorMode::LeftTop)
					position_collision = _position;
				else if (_achor_mode == AchorMode::BottomCentered)
					position_collision = { _position.x - _size.x / 2, _position.y - _size.y };

				if (!_is_invulnerable && bullet->get_valid()
					&& bullet->get_collide_target() == _id
					&& bullet->check_collision(position_collision, _size))
				{
					bullet->set_valid(false);
					bullet->on_collide();
					_hp -= bullet->get_damage();
					make_invulnerable();
					if (_hp <= 0)
					{
						_last_hurt_direction = _position - bullet->get_position();
						_velocity.x = _last_hurt_direction.x > 0 ? 0.35f : -0.35f;
						_velocity.y = -1.1f;
					}
				}
			}
		}

		// buff��ײ���
		for (Buff* buff : buff_list)
		{
			Vector2 position_collision;
			if (_achor_mode == AchorMode::LeftTop)
				position_collision = _position;
			else if (_achor_mode == AchorMode::BottomCentered)
				position_collision = { _position.x - _size.x / 2, _position.y - _size.y };

			if (buff->get_valid() && buff->check_collision(position_collision, _size))
			{
				buff->set_valid(false);
				_buff = buff->get_buff_id();
				buff->on_collide();

				if (_buff == BuffID::HURRY)
					_run_velocity = 0.8f;
				else if (_buff == BuffID::INVISIBLE)
					_is_invisible_player = true;
				_timer_buff.restart();
			}
		}
	}

	void make_invulnerable()
	{
		_is_invulnerable = true;
		_timer_invulnerable.restart();
	}

	virtual void on_attack()
	{}

	virtual void on_attack_ex()
	{}
};