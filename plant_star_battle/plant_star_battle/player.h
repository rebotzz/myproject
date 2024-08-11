#pragma once
#include <easyx.h>
#include "camera.h"
#include "animation.h"
#include "player_id.h"
#include "platform.h"
#include "bullet.h"
#include "particle.h"

extern bool is_debug;
extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

// ������Ч
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;
extern Atlas atlas_run_effect;


class Player
{
protected:
	// ��Ҷ���
	Animation _animation_idle_left;
	Animation _animation_idle_right;
	Animation _animation_run_left;
	Animation _animation_run_right;
	Animation _animation_attack_ex_left;		// ������⹥��������
	Animation _animation_attack_ex_right;		// ������⹥��������
	Animation* _current_animation = nullptr;	// ��ҵ�ǰ����
	PlayerID _id = PlayerID::P1;				// ������ID:���1,���2
	IMAGE _img_sketch;							// ����ܵ�����ʱ��ɫ��Ӱ

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

	// ����ģ��
	const float _run_velocity = 0.55f;			// ���ˮƽ�ƶ��ٶ�
	const float _jump_velocity = -0.85f;		// ����������ٶ�
	Vector2 _velocity;							// ��ҵ�ǰ��ά�ٶ�ʸ��
	const float _gravity = 1.6e-3f;				// ����������ٶ�: �Ի������Ϊ��
	Vector2 _size;								// �����ײ����
	Vector2 _position;							// ���λ��

	// ��Ч
	std::vector<Particle> _particle_run_effect;	// �ܶ�������Ч
	std::vector<Particle> _particle_die_effect;	// ����������Ч
	Animation _animation_jump_effect;			// ��Ծ��Ч����
	Animation _animation_land_effect;			// �����Ч����
	bool _is_run_effect_visiable = false;		// �ܶ�������Ч�Ƿ�ɼ�
	bool _is_jump_effect_visiable = false;		// ��Ծ��Ч�����Ƿ�ɼ�
	bool _is_land_effect_visiable = false;		// �����Ч�����Ƿ�ɼ�
	Timer _timer_particle_run_effect;			// �ܶ�������Ч��ʱ��(�䵱���ӷ�����)
	Timer _timer_particle_die_effect;			// ����������Ч��ʱ��(�䵱���ӷ�����)
	Vector2 _jump_effect_position;				// ��Ծ��Ч����λ��
	Vector2 _land_effect_position;				// �����Ч����λ��

public:
	Player()
	{
		_current_animation = &_animation_idle_right;

		_timer_attack_cd.set_one_shot(false);
		_timer_attack_cd.set_wait_time(_attack_cd);
		_timer_attack_cd.set_callback([&]() { _can_attack = true; });

		_timer_attack_ex.set_one_shot(true);
		_timer_attack_ex.set_wait_time(500);
		_timer_attack_ex.set_callback([&]() { _is_attack_ex = false; });

		_timer_invulnerable.set_wait_time(750); // �޵�֡ʱ��
		_timer_invulnerable.set_one_shot(true);
		_timer_invulnerable.set_callback([&]() { _is_invulnerable = false; });

		_timer_invulnerable_blink.set_wait_time(75);
		_timer_invulnerable_blink.set_one_shot(false);
		_timer_invulnerable_blink.set_callback([&]() { _is_showing_sketch_frame = !_is_showing_sketch_frame; });

		_timer_particle_run_effect.set_wait_time(70);
		_timer_particle_run_effect.set_one_shot(false);
		_timer_particle_run_effect.set_callback(
			[&]()
			{
				// ����ҽ�����������
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = _position.x + (_size.x - frame->getwidth()) / 2;
				particle_position.y = _position.y + _size.y - frame->getheight();
				_particle_run_effect.emplace_back(&atlas_run_effect, particle_position, 75);
			}
		);

		_timer_particle_die_effect.set_wait_time(50);
		_timer_particle_die_effect.set_one_shot(false);
		_timer_particle_die_effect.set_callback(
			[&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = _position.x + (_size.x - frame->getwidth()) / 2;
				particle_position.y = _position.y + _size.y - frame->getheight();
				_particle_die_effect.emplace_back(&atlas_run_effect, particle_position, 150);
			}
		);

		_animation_jump_effect.set_atlas(&atlas_jump_effect);
		_animation_land_effect.set_atlas(&atlas_land_effect);
		_animation_jump_effect.set_interval(75);
		_animation_land_effect.set_interval(75);
		_animation_jump_effect.set_loop(false);
		_animation_land_effect.set_loop(false);
		_animation_jump_effect.set_callback([&]() { _is_jump_effect_visiable = false; });
		_animation_land_effect.set_callback([&]() { _is_land_effect_visiable = false; });
	}
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
		int direction = _is_right_key_down - _is_left_key_down;

		if (direction != 0)
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
			_timer_particle_run_effect.pause();
		}

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
		_timer_particle_run_effect.on_update(interval_ms);
		if(_hp <= 0)
			_timer_particle_die_effect.on_update(interval_ms);

		_current_animation->on_update(interval_ms);

		move_and_collide(interval_ms);

		// ������Ч����
		if (_is_jump_effect_visiable)
			_animation_jump_effect.on_update(interval_ms);
		if (_is_land_effect_visiable)
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
	}

	virtual void on_draw(const Camera& camera) const
	{
		for (const Particle& particle : _particle_run_effect)
			if (particle.check_valid())
				particle.on_draw(camera);

		for (const Particle& particle : _particle_die_effect)
			if (particle.check_valid())
				particle.on_draw(camera);

		if (_is_jump_effect_visiable)
			_animation_jump_effect.on_draw(camera, (int)_jump_effect_position.x, (int)_jump_effect_position.y);
		if (_is_land_effect_visiable)
			_animation_land_effect.on_draw(camera, (int)_land_effect_position.x, (int)_land_effect_position.y);



		if (_hp > 0 && _is_invulnerable && _is_showing_sketch_frame)
			putimage_alpha(camera, (int)_position.x, (int)_position.y, (IMAGE*)&_img_sketch);
		else
			_current_animation->on_draw(camera, (int)_position.x, (int)_position.y);

		if (is_debug)
		{
			setlinecolor(RGB(0, 255, 255));
			rectangle(camera, (int)_position.x, (int)_position.y, (int)(_position.x + _size.x), (int)(_position.y + _size.y));
		}
	}

	const Vector2& get_position() const
	{
		return _position;
	}

	const Vector2& get_size() const
	{
		return _size;
	}

	int get_hp() const
	{
		return _hp;
	}

	int get_mp() const
	{
		return _mp;
	}

protected:
	void on_jump()
	{
		// ����������,ֻ����ƽ̨��ʱ������
		if (_velocity.y != 0 || _is_attack_ex)
			return;

		_velocity.y = _jump_velocity;

		_is_jump_effect_visiable = true;
		_animation_jump_effect.reset();
		IMAGE* frame = _animation_jump_effect.get_frame();
		_jump_effect_position.x = _position.x + (_size.x - frame->getwidth()) / 2;
		_jump_effect_position.y = _position.y + _size.y - frame->getheight();
	}

	void on_land()
	{
		_is_land_effect_visiable = true;
		_animation_land_effect.reset();
		IMAGE* frame = _animation_land_effect.get_frame();
		_land_effect_position.x = _position.x + (_size.x - frame->getwidth()) / 2;
		_land_effect_position.y = _position.y + _size.y - frame->getheight();
	}

	void on_run(float distance)
	{
		if (_is_attack_ex)
			return;

		_position.x += distance;

		_timer_particle_run_effect.resume();
	}

	// ��ײ������ģ��
	void move_and_collide(int interval_ms)
	{
		float last_tick_velocity_y = _velocity.y;
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
				if (bullet->get_valid() && bullet->get_collide_target() == _id
					&& bullet->check_collision(_position, _size))
				{
					bullet->set_valid(false);
					bullet->on_collide();
					_hp -= bullet->get_damage();
					make_invulnerable();
				}
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