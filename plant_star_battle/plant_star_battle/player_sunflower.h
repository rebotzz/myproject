#pragma once
#include "player.h"
#include <iostream>

using std::cout;
using std::endl;

// 向日葵
extern Atlas atlas_player_sunflower_idle_left;
extern Atlas atlas_player_sunflower_idle_right;
extern Atlas atlas_player_sunflower_run_left;
extern Atlas atlas_player_sunflower_run_right;

extern Atlas atlas_sunflower_sun;
extern Atlas atlas_sunflower_sun_explode;
extern Atlas atlas_sunflower_sun_ex;
extern Atlas atlas_sunflower_sun_ex_explode;
extern Atlas atlas_sunflower_sun_text;
extern Atlas atlas_player_sunflower_attack_ex_right;
extern Atlas atlas_player_sunflower_die_right;
extern Atlas atlas_player_sunflower_attack_ex_left;
extern Atlas atlas_player_sunflower_die_left;


class PlayerSunflower : public Player
{
private:

public:
	PlayerSunflower()
	{
		// 初始化
		_animation_idle_left.set_atlas(&atlas_player_sunflower_idle_left);
		_animation_idle_right.set_atlas(&atlas_player_sunflower_idle_right);
		_animation_run_left.set_atlas(&atlas_player_sunflower_run_left);
		_animation_run_right.set_atlas(&atlas_player_sunflower_run_right);

		_animation_idle_left.set_interval(75);
		_animation_idle_right.set_interval(75);
		_animation_run_left.set_interval(75);
		_animation_run_right.set_interval(75);

		_animation_idle_left.set_loop(true);
		_animation_idle_right.set_loop(true);
		_animation_run_left.set_loop(true);
		_animation_run_right.set_loop(true);

		_current_animation = &_animation_idle_left;
		
		// 玩家碰撞矩形
		_size.x = 96;
		_size.y = 96;
	}

	~PlayerSunflower()
	{}

	virtual void on_update(int interval_ms) override
	{
		//cout << "向日葵on_update" << endl;
		Player::on_update(interval_ms);
	}

	virtual void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);
	}

};