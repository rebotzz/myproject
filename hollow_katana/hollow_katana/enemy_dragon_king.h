#pragma once
#include "character.h"

class EnemyDragonKing : public Character
{
private:

	const float SPEED_RUN = 400.0f;
	const float SPEED_JUMP = 1200.0f;
	const float SPEED_DASH = 1500.0f;

private:
	bool is_dashing = false;

	AnimationGroup animation_dash_vfx;
	Animation* current_dash_animation = nullptr;

	Animation animation_vfx_slash_left;								// 攻击刀光特效动画,左
	Animation animation_vfx_slash_right;							// 攻击刀光特效动画,右
	Animation* current_slash_animation = nullptr;					// 当前刀光动画



public:
	EnemyDragonKing();
	~EnemyDragonKing();
	virtual void on_hurt() override;

	virtual void on_input(const ExMessage& msg) override { };
	virtual void on_update(float delta) override;
	virtual void on_render() override;


	void on_dash();


	// my add 
	void on_jump()
	{
		velocity.y -= SPEED_JUMP;
	}
	void on_move_left(bool is_left)
	{
		velocity.x = is_left ? -SPEED_RUN : SPEED_RUN;
	}
	void set_dash_speed(const Vector2& dash_dir_normalize)
	{
		velocity = dash_dir_normalize.normalize() * SPEED_DASH;
	}

	void set_facing_left(bool flag)
	{
		is_facing_left = flag;
	}
	bool get_facing_left() const
	{
		return is_facing_left;
	}


};