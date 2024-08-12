#pragma once
#include "player.h"
#include "bubbles_bullet.h"
#include "bubbles_ex_bullet.h"

extern Camera main_camera;

// 忧郁蘑菇
extern Atlas atlas_player_gloomshroom_attack_ex_right;
extern Atlas atlas_player_gloomshroom_die_right;
extern Atlas atlas_player_gloomshroom_idle_right;
extern Atlas atlas_player_gloomshroom_run_right;

extern Atlas atlas_player_gloomshroom_attack_ex_left;
extern Atlas atlas_player_gloomshroom_die_left;
extern Atlas atlas_player_gloomshroom_idle_left;
extern Atlas atlas_player_gloomshroom_run_left;


class PlayerGloomshroom : public Player
{
private:
	Timer _timer_spawn_bubbles_ex;		// 特殊攻击生成泡泡定时器

public:
	PlayerGloomshroom(bool face_right = true) :Player(face_right)
	{
		// 玩家碰撞矩形
		_size.x = 96;
		_size.y = 96;

		// 动画资源初始化
		_animation_idle_left.set_atlas(&atlas_player_gloomshroom_idle_left);
		_animation_idle_right.set_atlas(&atlas_player_gloomshroom_idle_right);
		_animation_run_left.set_atlas(&atlas_player_gloomshroom_run_left);
		_animation_run_right.set_atlas(&atlas_player_gloomshroom_run_right);
		_animation_attack_ex_left.set_atlas(&atlas_player_gloomshroom_attack_ex_left);
		_animation_attack_ex_right.set_atlas(&atlas_player_gloomshroom_attack_ex_right);
		_animation_die_left.set_atlas(&atlas_player_gloomshroom_die_left);
		_animation_die_right.set_atlas(&atlas_player_gloomshroom_die_right);

		_animation_idle_left.set_interval(75);
		_animation_idle_right.set_interval(75);
		_animation_run_left.set_interval(75);
		_animation_run_right.set_interval(75);
		_animation_die_left.set_interval(150);
		_animation_die_right.set_interval(150);
		_animation_attack_ex_left.set_interval(75);
		_animation_attack_ex_right.set_interval(75);

		_animation_idle_left.set_loop(true);
		_animation_idle_right.set_loop(true);
		_animation_run_left.set_loop(true);
		_animation_run_right.set_loop(true);
		_animation_die_left.set_loop(false);
		_animation_die_right.set_loop(false);
		_animation_attack_ex_left.set_loop(true);
		_animation_attack_ex_right.set_loop(true);

		_attack_cd = 400;
		_timer_attack_cd.set_wait_time(_attack_cd);
		_timer_attack_ex.set_wait_time(2500);		// 大招持续2.5s

		_timer_spawn_bubbles_ex.set_wait_time(500);
		_timer_spawn_bubbles_ex.set_one_shot(false);
		_timer_spawn_bubbles_ex.set_callback([&]()
			{
				spawn_bubbles(true);
			}
		);
	}

	~PlayerGloomshroom()
	{}

	virtual void on_update(int interval_ms) override
	{
		// 碰撞检测,物理检测,动画更新等逻辑
		Player::on_update(interval_ms);

		if (_is_attack_ex)
		{
			_timer_spawn_bubbles_ex.on_update(interval_ms);
		}

	}

	virtual void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);
	}

private:
	virtual void on_attack() override
	{
		spawn_bubbles(false);
		mciSendString(L"play bubbles_shot from 0", nullptr, 0, nullptr);
	}

	virtual void on_attack_ex() override
	{
		spawn_bubbles(true);
		_timer_spawn_bubbles_ex.restart(); 
		_is_face_right ? _animation_attack_ex_right.reset() : _animation_attack_ex_left.reset();
		mciSendString(L"play bubbles_shot_ex from 0", nullptr, 0, nullptr);
	}

	void spawn_bubbles(bool bubbles_ex = false)
	{
		Bullet* bubbles_bullet = nullptr;
		if(bubbles_ex)
			bubbles_bullet = new BubblesExBullet;
		else
			bubbles_bullet = new BubblesBullet;

		Vector2 bullet_position;
		const Vector2& bullet_size = bubbles_bullet->get_size();
		bullet_position.x = _position.x + (_size.x - bullet_size.x) / 2;
		bullet_position.y = _position.y + (_size.y - bullet_size.y) / 2;
		bubbles_bullet->set_collide_target(_id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		bubbles_bullet->set_position(bullet_position);
		bubbles_bullet->set_velocity({ 0, 0 });
		bubbles_bullet->set_damage(15);
		bubbles_bullet->set_callback([&]()
			{
				_mp += 25;
			}
		);

		bullet_list.push_back(bubbles_bullet);

	}

};
