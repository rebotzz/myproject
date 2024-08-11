#pragma once
#include "player.h"
#include "pea_bullet.h"

// 豌豆射手
extern Atlas atlas_player_peashooter_idle_right;
extern Atlas atlas_player_peashooter_run_right;
extern Atlas atlas_player_peashooter_idle_left;
extern Atlas atlas_player_peashooter_run_left;

extern Atlas atlas_player_peashooter_attack_ex_right;
extern Atlas atlas_player_peashooter_attack_ex_left;
extern Atlas atlas_player_peashooter_die_left;
extern Atlas atlas_player_peashooter_die_right;


class PlayerPeashoter : public Player
{
private:
	float _pea_bullet_speed = 0.75f;			// 豌豆射手普攻子弹速度
	float _pea_bullet_ex_speed = 1.5f;			// 豌豆射手特殊攻击子弹速度

	int _attack_ex_duration = 2500;				// 特殊攻击持续时间
	Timer _timer_spawn_pea_ex;					// 特殊攻击时,豌豆子弹发射计时器

public:
	PlayerPeashoter()
	{
		// 初始化
		_animation_idle_left.set_atlas(&atlas_player_peashooter_idle_left);
		_animation_idle_right.set_atlas(&atlas_player_peashooter_idle_right);
		_animation_run_left.set_atlas(&atlas_player_peashooter_run_left);
		_animation_run_right.set_atlas(&atlas_player_peashooter_run_right);
		_animation_attack_ex_left.set_atlas(&atlas_player_peashooter_attack_ex_left);
		_animation_attack_ex_right.set_atlas(&atlas_player_peashooter_attack_ex_right);

		_animation_idle_left.set_interval(75);
		_animation_idle_right.set_interval(75);
		_animation_run_left.set_interval(75);
		_animation_run_right.set_interval(75);
		_animation_attack_ex_left.set_interval(75);
		_animation_attack_ex_right.set_interval(75);

		_animation_idle_left.set_loop(true);
		_animation_idle_right.set_loop(true);
		_animation_run_left.set_loop(true);
		_animation_run_right.set_loop(true);
		_animation_attack_ex_left.set_loop(true);
		_animation_attack_ex_right.set_loop(true);

		// 玩家碰撞矩形
		_size.x = 96;
		_size.y = 96;

		// 基类构造在派生类构造之前, 需要更改计时器wait_time
		_attack_cd = 250;	
		_timer_attack_cd.set_wait_time(_attack_cd);
		_timer_attack_ex.set_wait_time(_attack_ex_duration);

		_timer_spawn_pea_ex.set_wait_time(100);
		_timer_spawn_pea_ex.set_one_shot(false);
		_timer_spawn_pea_ex.set_callback(
			[&]()
			{
				spawn_pea_bullet(_pea_bullet_ex_speed);
			}
		);
	}

	~PlayerPeashoter()
	{}

	virtual void on_update(int interval_ms) override
	{
		Player::on_update(interval_ms);

		if (_is_attack_ex)
		{
			main_camera.shake(5, 100);
			_timer_spawn_pea_ex.on_update(interval_ms);
		}
	}

	virtual void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);
	}

private:
	virtual void on_attack() override
	{
		spawn_pea_bullet(_pea_bullet_speed);

		switch (rand() % 2)
		{
		case 0:
			mciSendString(L"play pea_shoot_1 from 0", nullptr, 0, nullptr);
			break;
		case 1:
			mciSendString(L"play pea_shoot_2 from 0", nullptr, 0, nullptr);
			break;
		}
	}

	virtual void on_attack_ex() override
	{
		_is_face_right ? _animation_attack_ex_right.reset() : _animation_attack_ex_left.reset();

		mciSendString(L"play pea_shoot_ex from 0", nullptr, 0, nullptr);
	}

	void spawn_pea_bullet(float speed)
	{
		PeaBullet* pea_bullet = new PeaBullet;
		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = pea_bullet->get_size();
		// 子弹生成位置在豌豆射手枪口
		bullet_position.x = _is_face_right ?
			_position.x + _size.x - bullet_size.x
			: _position.x/* + bullet_size.x*/;
		bullet_position.y = _position.y;

		bullet_velocity.x = _is_face_right ? speed : -speed;
		pea_bullet->set_collide_target(_id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);

		pea_bullet->set_callback([&]() { _mp += 20; });
		pea_bullet->set_position(bullet_position);
		pea_bullet->set_velocity(bullet_velocity);

		bullet_list.push_back(pea_bullet);
	}
};
