#pragma once
#include <vector>
#include "character.h"
#include "sword.h"
#include "barb.h"

class EnemyHornet : public Character
{
	friend class HornetControl;
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

	virtual void on_update(float delta) override;
	virtual void on_render() override;

	virtual void on_hurt() override;
	void on_dash();
	void on_throw_silk();
	void throw_sword();
	void throw_barbs();

	// add 
	void set_dashing_in_air(bool flag) { is_dashing_in_air = flag; }
	void set_dashing_on_floor(bool flag) { is_dashing_on_floor = flag; }
	void set_throwing_silk(bool flag) { is_throwing_silk = flag; }


	virtual void move(float delta);
	virtual bool can_skill_1() const { return is_skill_1_key_down; }
	virtual bool can_skill_2() const { return is_skill_2_key_down; }
	virtual void release_skill_1() { switch_state("dash_on_floor"); }
	virtual void release_skill_2() { switch_state("throw_silk"); }

	virtual void switch_to_human_control() override;
	virtual void switch_to_auto_control() override;
};