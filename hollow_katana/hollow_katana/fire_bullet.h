#pragma once
#include "vector2.h"
#include "animation.h"
#include "collision_box.h"


class FireBullet
{
private:
	Vector2 _position;
	Vector2 velocity;
	const float SPEED = 1200.f;
	Timer timer_trigger;
	bool is_trigger = false;
	CollisionBox* collision_box = nullptr;
	Animation animation;
	bool is_valid = true;

public:
	FireBullet();
	~FireBullet();
	bool check_valid() const { return is_valid; }

	void on_update(float delta);
	void on_render();
	void set_position(const Vector2& position) { _position = position; }
	const Vector2& get_position() {  return _position; }
	void set_trigger_time(float delta) { timer_trigger.set_wait_time(delta); }
	bool get_trigger() const { return is_trigger; }
};

