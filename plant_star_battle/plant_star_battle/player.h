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

// 动作特效
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;
extern Atlas atlas_run_effect;

class Player
{
public:
	// 玩家锚点位置
	enum class AchorMode
	{
		Centered,
		BottomCentered,
		LeftTop			// 为了兼容之前的
	};

protected:
	// 玩家动画
	Animation _animation_idle_left;				// 玩家休闲动画左
	Animation _animation_idle_right;			// 玩家休闲动画右
	Animation _animation_run_left;				// 玩家奔跑动画左
	Animation _animation_run_right;				// 玩家奔跑动画右
	Animation _animation_attack_ex_left;		// 玩家特殊攻击动画左
	Animation _animation_attack_ex_right;		// 玩家特殊攻击动画右
	Animation _animation_die_left;				// 玩家死亡动画左
	Animation _animation_die_right;				// 玩家死亡动画右
	Animation* _current_animation = nullptr;	// 玩家当前动画
	PlayerID _id = PlayerID::P1;				// 玩家序号ID:玩家1,玩家2
	IMAGE _img_sketch;							// 玩家受到攻击时白色剪影
	bool _is_update_animation = true;			// 避免子类玩家动画和技能有关的情况

	// 玩家移动状态
	bool _is_left_key_down = false;				// 玩家是否向左移动
	bool _is_right_key_down = false;			// 玩家是否向右移动
	bool _is_face_right = true;					// 玩家默认朝向

	// 玩家状态栏
	int _hp = 100;								// 玩家血条
	int _mp = 0;								// 玩家能量条
	bool _can_attack = true;					// 能否普通攻击
	bool _is_attack_ex = false;					// 是否在释放特殊攻击
	bool _is_invulnerable = false;				// 玩家是否处在无敌状态
	bool _is_showing_sketch_frame = false;		// 是否播放玩家受击时白色剪影
	int _attack_cd = 500;						// 普通攻击cd
	Timer _timer_attack_cd;						// 普通攻击计时器
	Timer _timer_attack_ex;						// 特殊攻击状态定时器
	Timer _timer_invulnerable;					// 无敌帧定时器
	Timer _timer_invulnerable_blink;			// 玩家受击时白色剪影闪烁播放定时器
	Timer _timer_buff;							// 玩家buff持续时间计时器
	Timer _timer_buff_recovery;					// 玩家恢复hp/mp定时器
	BuffID _buff = BuffID::INVALID;				// 玩家当前buff
	bool _is_invisible_player = false;			// 玩家是否不可见

	// 物理模拟
	float _run_velocity = 0.55f;				// 玩家水平移动速度
	float _jump_velocity = -0.85f;				// 玩家起跳初速度
	Vector2 _velocity;							// 玩家当前二维速度矢量
	float _gravity = 1.6e-3f;					// 玩家重力加速度: 以画面表现为主
	Vector2 _size;								// 玩家碰撞矩形
	Vector2 _position;							// 玩家位置
	AchorMode _achor_mode = AchorMode::LeftTop;	// 描述玩家位置的锚点模式

	// 特效
	std::vector<Particle> _particle_run_effect;	// 跑动粒子特效
	std::vector<Particle> _particle_die_effect;	// 死亡粒子特效
	Timer _timer_run_particle_effect_generation;// 跑动粒子特效定时器(充当粒子发射器)
	Timer _timer_die_particle_effect_generation;// 死亡粒子特效定时器(充当粒子发射器)
	Animation _animation_jump_effect;			// 跳跃特效动画
	Animation _animation_land_effect;			// 落地特效动画
	bool _is_jump_effect_visible = false;		// 跳跃特效动画是否可见
	bool _is_land_effect_visible = false;		// 落地特效动画是否可见
	Vector2 _position_jump_effect;				// 跳跃特效坐标位置
	Vector2 _position_land_effect;				// 落地特效坐标位置
	Vector2 _last_hurt_direction;				// 最后一次中弹方向

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

		_timer_invulnerable.set_wait_time(500); // 无敌帧时长
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
				// 在玩家脚下生成粒子
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
				// 恢复玩家原本状态,去除临时状态
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

		// 不同玩家ID对应按键不同
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
		// 角色位置更新
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
		// 左右边界
		if (_position.x < (_achor_mode == AchorMode::LeftTop ? 0 : _size.x / 2))
			_position.x = (_achor_mode == AchorMode::LeftTop ? 0 : _size.x / 2);
		else if (_position.x > (getwidth() - (_achor_mode == AchorMode::LeftTop ? _size.x : _size.x / 2)))
			_position.x = (getwidth() - (_achor_mode == AchorMode::LeftTop ? _size.x : _size.x / 2));

		// 如果角色死亡
		if (_hp <= 0)
			_current_animation = _last_hurt_direction.x > 0 ? &_animation_die_left : &_animation_die_right;

		// 更新计时器
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

		// 更新当前动画
		if(_is_update_animation)
			_current_animation->on_update(interval_ms);

		// 物理判定检测
		move_and_collide(interval_ms);

		// 更新特效动画
		if (_is_jump_effect_visible)
			_animation_jump_effect.on_update(interval_ms);
		if (_is_land_effect_visible)
			_animation_land_effect.on_update(interval_ms);

		// 删除无效粒子
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

		// 更新粒子状态
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
		// 粒子特效
		for (const Particle& particle : _particle_run_effect)
			if (particle.check_valid())
				particle.on_draw(camera);

		for (const Particle& particle : _particle_die_effect)
			if (particle.check_valid())
				particle.on_draw(camera);

		// 跳跃/落地特效
		if (_is_jump_effect_visible)
			_animation_jump_effect.on_draw(camera, (int)_position_jump_effect.x, (int)_position_jump_effect.y);
		if (_is_land_effect_visible)
			_animation_land_effect.on_draw(camera, (int)_position_land_effect.x, (int)_position_land_effect.y);


		// 玩家动画
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

			// 玩家碰撞边框
			setlinecolor(RGB(0, 255, 255));
			rectangle(camera, (int)position_collision.x, (int)position_collision.y,
				(int)(position_collision.x + _size.x), (int)(position_collision.y + _size.y));

			// 玩家动画帧边框
			setlinecolor(RGB(0, 250, 154));
			rectangle(camera, (int)position_draw.x, (int)position_draw.y,
				(int)(position_draw.x + frame->getwidth()), (int)(position_draw.y + frame->getheight()));

			// 玩家位置
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
		// 避免无限跳,只有在平台上时才能跳
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

	// 碰撞和物理模拟		
	void move_and_collide(int interval_ms)
	{
		float last_tick_velocity_y = _velocity.y;
		_velocity.y += _gravity * (float)interval_ms;
		_position += _velocity * (float)interval_ms;

		if (_hp <= 0)
			return;

		// 单向平台碰撞检测: 下落时,y轴向下		bug: 玩家初始进入游戏时玩家有概率掉落平台下, float精度? 速度过快来不及检测?
		if (_velocity.y >= 0)
		{
			for (const Platform& platform : platform_list)
			{
				// x轴,y轴碰撞检测
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
					// 条件:玩家上一帧脚底高于平台,避免下落过程中闪现回平台
					float last_tick_pos_bottom_y = 0;
					if (_achor_mode == AchorMode::LeftTop)
						last_tick_pos_bottom_y = _position.y + _size.y - _velocity.y * interval_ms;
					else if (_achor_mode == AchorMode::BottomCentered)
						last_tick_pos_bottom_y = _position.y - _velocity.y * interval_ms;

					static const float delta_y = 0.0001f;				// 避免误判
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

		// 子弹碰撞检测
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

		// buff碰撞检测
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