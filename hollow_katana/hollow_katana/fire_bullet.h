#pragma once
#include "vector2.h"
#include "animation.h"
#include "collision_box.h"


class FireBullet
{
private:
	const float SPEED = 1200.f;
	const float RADIAL_SPEED = 3.2f;						// 子弹的径向波动速度
	const float TANGENT_SPEED = 5.2f;						// 子弹的切向旋转速度

private:
	Vector2 base_position;
	Vector2 current_position;
	Vector2 velocity;
	CollisionBox* collision_box = nullptr;
	Animation animation;
	Timer timer_trigger;
	bool is_valid = true;
	bool is_trigger = false;
	float pass_time = 0.f;
	float init_angle = 0.f;

public:
	FireBullet();
	~FireBullet();

	void on_update(float delta);
	void on_render();
	bool check_valid() const { return is_valid; }
	void set_position(const Vector2& position) { current_position = position; }
	const Vector2& get_position() { return current_position; }
	void set_trigger_time(float delta) { timer_trigger.set_wait_time(delta); }
	void set_base_position(const Vector2& position) { base_position = position; }
	void set_init_angle(float angle) { init_angle = angle; }

private:
	void update_circle(float delta)
	{
		pass_time += delta;
		float radius = 120.f + sin(pass_time * RADIAL_SPEED) * 30;		// 子弹到角色半径
		float radians = pass_time * TANGENT_SPEED + init_angle;			// 子弹与角色间角度
		current_position = base_position + Vector2(float(cos(radians) * radius), float(sin(radians) * radius));
	}
};

