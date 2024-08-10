#pragma once
#include "player.h"
#include <iostream>

using std::cout;
using std::endl;

// Õ„∂π…‰ ÷
extern Atlas atlas_player_peashooter_idle_right;
extern Atlas atlas_player_peashooter_run_right;
extern Atlas atlas_player_peashooter_idle_left;
extern Atlas atlas_player_peashooter_run_left;

extern IMAGE img_pea_bullet;
extern Atlas atlas_pea_bullet_break;
extern Atlas atlas_player_peashooter_attack_ex_right;
extern Atlas atlas_player_peashooter_die_right;
extern Atlas atlas_player_peashooter_attack_ex_left;
extern Atlas atlas_player_peashooter_die_left;


class PlayerPeashoter : public Player
{
private:

public:
	PlayerPeashoter()
	{
		// ≥ı ºªØ
		_animation_idle_left.set_atlas(&atlas_player_peashooter_idle_left);
		_animation_idle_right.set_atlas(&atlas_player_peashooter_idle_right);
		_animation_run_left.set_atlas(&atlas_player_peashooter_run_left);
		_animation_run_right.set_atlas(&atlas_player_peashooter_run_right);

		_animation_idle_left.set_interval(75);
		_animation_idle_right.set_interval(75);
		_animation_run_left.set_interval(75);
		_animation_run_right.set_interval(75);

		_animation_idle_left.set_loop(true);
		_animation_idle_right.set_loop(true);
		_animation_run_left.set_loop(true);
		_animation_run_right.set_loop(true);

		_current_animation = &_animation_idle_left;
		// ÕÊº“≈ˆ◊≤æÿ–Œ
		_size.x = 96;
		_size.y = 96;
	}

	~PlayerPeashoter()
	{}

	virtual void on_update(int interval_ms) override
	{
		//cout << "Õ„∂π…‰ ÷on_update" << endl;
		Player::on_update(interval_ms);
	}

	virtual void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);
	}

};
