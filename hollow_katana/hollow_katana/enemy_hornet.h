#pragma once
#include <vector>
#include "character.h"
#include "collision_box.h"
#include "sword.h"
#include "barb.h"

class EnemyHornet : public Character
{
private:
	bool is_dashing_in_air = false;
	bool is_dashing_on_floor = false;
	bool is_throwing_silk = false;

	Animation animation_silk;
	AnimationGroup animation_dash_in_air_vfx;
	AnimationGroup animation_dash_on_floor_vfx;
	Animation* current_dash_animation = nullptr;

	CollisionBox* collision_box_silk = nullptr;
	std::vector<Barb*> barb_list;
	std::vector<Sword*> sword_list;

public:
	EnemyHornet();
	~EnemyHornet();
	virtual void on_hurt() override;

	virtual void on_input(const ExMessage& msg) override;
	virtual void on_update(float delta) override;
	virtual void on_render() override;

	void throw_sword();
	void throw_barbs();

	void on_dash();
	void on_throw_silk();

	void set_facing_left(bool flag)
	{
		is_facing_left = flag;
	}
	bool get_facing_left() const
	{
		return is_facing_left;
	}

	void set_dashing_in_air(bool flag)
	{
		is_dashing_in_air = flag;
	}
	bool get_dashing_in_air() const
	{
		return is_dashing_in_air;
	}

	void set_dashing_on_floor(bool flag)
	{
		is_dashing_on_floor = flag;
	}
	bool get_dashing_on_floor() const
	{
		return is_dashing_on_floor;
	}

	void set_throwing_silk(bool flag)
	{
		is_throwing_silk = flag;
	}
	bool get_throwing_silk() const
	{
		return is_throwing_silk;
	}

};