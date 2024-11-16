#pragma once
#include "player.h"
#include "sword_bullet.h"
#include "silk_bullet.h"

// 空洞骑士:大黄蜂(小姐姐)
extern Atlas atlas_player_hornet_idle_left;
extern Atlas atlas_player_hornet_idle_right;
extern Atlas atlas_player_hornet_run_left;
extern Atlas atlas_player_hornet_run_right;
extern Atlas atlas_player_hornet_jump_right;
extern Atlas atlas_player_hornet_jump_left;
extern Atlas atlas_player_hornet_fall_left;
extern Atlas atlas_player_hornet_fall_right;

extern Atlas atlas_player_hornet_throw_silk_left;
extern Atlas atlas_player_hornet_throw_silk_right;
extern Atlas atlas_player_hornet_throw_sword_left;
extern Atlas atlas_player_hornet_throw_sword_right;
extern Atlas atlas_hornet_silk;
extern Atlas atlas_hornet_sword_left;
extern Atlas atlas_hornet_sword_right;

class PlayerHornet : public Player
{
private:
	Timer _timer_start_attack_ex;
	Timer _timer_stay_sky;				// 滞空定时器
	bool _is_stay_sky = false;
	float _sword_bullet_speed = 1.35f;
	int _stay_sky_duration = 1000;
	bool _is_attack = false;

	Animation _animation_jump_left;
	Animation _animation_jump_right;
	Animation _animation_fall_left;
	Animation _animation_fall_right;
	Animation _animation_throw_sword_left;
	Animation _animation_throw_sword_right;

public:
	PlayerHornet(bool face_right = true) :Player(face_right)
	{
		_achor_mode = AchorMode::BottomCentered;
		_is_update_animation = false;

		// 玩家碰撞矩形
		_size.x = 75;
		_size.y = 130;

		// 动画资源初始化
		_animation_idle_left.set_atlas(&atlas_player_hornet_idle_left);
		_animation_idle_right.set_atlas(&atlas_player_hornet_idle_right);
		_animation_run_left.set_atlas(&atlas_player_hornet_run_left);
		_animation_run_right.set_atlas(&atlas_player_hornet_run_right);
		_animation_attack_ex_left.set_atlas(&atlas_player_hornet_throw_silk_left);
		_animation_attack_ex_right.set_atlas(&atlas_player_hornet_throw_silk_right);

		_animation_jump_left.set_atlas(&atlas_player_hornet_jump_left);
		_animation_jump_right.set_atlas(&atlas_player_hornet_jump_right);
		_animation_fall_left.set_atlas(&atlas_player_hornet_fall_left);
		_animation_fall_right.set_atlas(&atlas_player_hornet_fall_right);
		_animation_throw_sword_left.set_atlas(&atlas_player_hornet_throw_sword_left);
		_animation_throw_sword_right.set_atlas(&atlas_player_hornet_throw_sword_right);

		_animation_idle_left.set_interval(75);
		_animation_idle_right.set_interval(75);
		_animation_run_left.set_interval(75);
		_animation_run_right.set_interval(75);
		_animation_fall_left.set_interval(75);
		_animation_fall_right.set_interval(75);

		_animation_jump_left.set_interval(45);
		_animation_jump_right.set_interval(45);
		_animation_throw_sword_left.set_interval(30);
		_animation_throw_sword_right.set_interval(30);
		_animation_attack_ex_left.set_interval(75);
		_animation_attack_ex_right.set_interval(75);

		_animation_idle_left.set_loop(true);
		_animation_idle_right.set_loop(true);
		_animation_run_left.set_loop(true);
		_animation_run_right.set_loop(true);
		_animation_fall_left.set_loop(true);
		_animation_fall_right.set_loop(true);

		_animation_jump_left.set_loop(false);
		_animation_jump_right.set_loop(false);
		_animation_throw_sword_left.set_loop(false);
		_animation_throw_sword_right.set_loop(false);
		_animation_attack_ex_left.set_loop(false);
		_animation_attack_ex_right.set_loop(false);

		_animation_jump_left.set_callback([&]() {});
		_animation_jump_right.set_callback([&]() {});

		_animation_attack_ex_left.set_callback([&]()
			{
				_is_attack_ex = false;
			});
		_animation_attack_ex_right.set_callback([&]()
			{
				_is_attack_ex = false;	
			});

		_animation_throw_sword_left.set_callback([&]()
			{
				_is_attack = false;
				spawn_sword();
				mciSendString(L"play hornet_throw_sword from 0", nullptr, 0, nullptr);
			}
		);
		_animation_throw_sword_right.set_callback([&]()
			{
				_is_attack = false;
				spawn_sword();
				mciSendString(L"play hornet_throw_sword from 0", nullptr, 0, nullptr);
			}
		);

		_attack_cd = 500;
		_timer_attack_cd.set_wait_time(_attack_cd);
		_timer_attack_ex.set_callback([]() {});
		_timer_stay_sky.set_wait_time(_stay_sky_duration);
		_timer_stay_sky.set_one_shot(true);
		_timer_stay_sky.set_callback([&]()
			{
				_is_stay_sky = false;
			});

		_timer_start_attack_ex.set_wait_time(300);
		_timer_start_attack_ex.set_one_shot(true);
		_timer_start_attack_ex.set_callback([&]()
			{
				spawn_silk();
				mciSendString(L"play hornet_throw_silk from 0", nullptr, 0, nullptr);
			});

		_animation_die_left = _animation_fall_left;
		_animation_die_right = _animation_fall_right;
	}
	~PlayerHornet() = default;

	virtual void on_update(int interval_ms) override
	{
		int last_tick_hp = _hp;

		// 碰撞检测,物理检测,动画更新等逻辑
		Player::on_update(interval_ms);

		int current_hp = _hp;
		if (current_hp < last_tick_hp)
		{
			switch (rand() % 3)
			{
			case 0:
				mciSendString(L"play hornet_hurt_1 from 0", nullptr, 0, nullptr);
				break;
			case 1:
				mciSendString(L"play hornet_hurt_2 from 0", nullptr, 0, nullptr);
				break;
			case 2:
				mciSendString(L"play hornet_hurt_3 from 0", nullptr, 0, nullptr);
				break;
			}
		}

		if (_is_attack_ex)
			_timer_start_attack_ex.on_update(interval_ms);

		if (_is_stay_sky)
			_timer_stay_sky.on_update(interval_ms);

		// 大招期间保持浮空
		if (_is_stay_sky && _velocity.y > 0)
			_velocity.y = 0;

		// 不同状态动画切换
		if (_is_attack_ex)
			_current_animation = _is_face_right ? &_animation_attack_ex_right : &_animation_attack_ex_left;
		else if (_is_attack)
			_current_animation = _is_face_right ? &_animation_throw_sword_right : &_animation_throw_sword_left;
		else if (_velocity.y < 0)
			_current_animation = _is_face_right ? &_animation_jump_right : &_animation_jump_left;
		else if (_velocity.y > 0)
			_current_animation = _is_face_right ? &_animation_fall_right : &_animation_fall_left;

		_current_animation->on_update(interval_ms);
	}

	virtual void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);
		_is_face_right ? _animation_jump_right.reset() : _animation_jump_left.reset();
	}

private:
	virtual void on_attack() override
	{
		_is_attack = true;
		_is_face_right ? _animation_throw_sword_right.reset() : _animation_throw_sword_left.reset();

		mciSendString(L"play hornet_say_throw_sword from 0", nullptr, 0, nullptr);
	}

	virtual void on_attack_ex() override
	{
		_is_face_right ? _animation_attack_ex_right.reset() : _animation_attack_ex_left.reset();
		if (_velocity.y < 0)
		{
			_timer_stay_sky.restart();
			_is_stay_sky = true;
		}
		_timer_start_attack_ex.restart();

		switch (rand() % 2)
		{
		case 0 :
			mciSendString(L"play hornet_say_throw_silk_1 from 0", nullptr, 0, nullptr);
			break;
		case 1:
			mciSendString(L"play hornet_say_throw_silk_2 from 0", nullptr, 0, nullptr);
			break;
		}
	}

	virtual void on_jump() override
	{
		Player::on_jump();
		_is_face_right ? _animation_jump_right.reset() : _animation_jump_left.reset();
	}


	void spawn_sword()
	{
		Bullet* sword_bullet = new SwordBullet(!_is_face_right);

		Vector2 bullet_position;
		const Vector2& bullet_size = sword_bullet->get_size();
		bullet_position.x = _position.x - bullet_size.x / 2 + (_is_face_right ? 40 : -40);
		bullet_position.y = _position.y - (_size.y - bullet_size.y) / 2;
		sword_bullet->set_collide_target(_id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		sword_bullet->set_position(bullet_position);
		sword_bullet->set_velocity({ _is_face_right ? _sword_bullet_speed : -_sword_bullet_speed, 0 });
		sword_bullet->set_damage(20);
		sword_bullet->set_callback([&]()
			{
				_mp += 25;
			}
		);

		bullet_list.push_back(sword_bullet);
	}

	void spawn_silk()
	{
		Bullet* silk_bullet = new SilkBullet;
		Vector2 bullet_position;
		const Vector2& bullet_size = silk_bullet->get_size();
		bullet_position.x = _position.x - bullet_size.x / 2;
		bullet_position.y = _position.y - _size.y / 2 - bullet_size.y / 2;
		silk_bullet->set_collide_target(_id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		silk_bullet->set_position(bullet_position);
		silk_bullet->set_velocity({ 0, 0 });
		silk_bullet->set_damage(15);
		silk_bullet->set_callback([&]()
			{
				_mp += 25;
			}
		);

		bullet_list.push_back(silk_bullet);
	}
};