#pragma once
#include "vector2.h"
#include "animation.h"
#include "collision_box.h"

//class Bullet
//{
//private:
//	Vector2 _position;
//	Vector2 velocity;
//	CollisionBox* collision_box = nullptr;
//	Animation animation;
//	bool is_valid = true;
//
//private:
//	const float SPEED_MOVE = 1250.0f;
//
//public:
//	Bullet(const Vector2& postion_src, bool move_left);
//	~Bullet();
//	bool check_valid() const { return is_valid; }
//
//	void on_update(float delta);
//	void on_render();
//};


class FireBullet
{
private:
	Vector2 _position;
	Vector2 velocity;
	const float SPEED = 1500.f;
	Timer timer_trigger;
	bool is_trigger = false;
	CollisionBox* collision_box = nullptr;
	Animation animation;
	bool is_valid = true;

private:
	const float SPEED_MOVE = 1250.0f;

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


class FireDash
{
private:
	Vector2 _position;
	Vector2 velocity;
	CollisionBox* collision_box = nullptr;
	Animation animation;
	bool is_valid = true;

private:
	const float SPEED_MOVE = 1250.0f;

public:
	FireDash(const Vector2& postion_src, bool move_left);
	~FireDash();
	bool check_valid() const { return is_valid; }

	void on_update(float delta);
	void on_render();
};