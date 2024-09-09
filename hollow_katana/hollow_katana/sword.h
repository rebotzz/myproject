#pragma once
#include "vector2.h"
#include "animation.h"
#include "collision_box.h"

class Sword
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
	Sword(const Vector2& postion_src, bool move_left);
	~Sword();
	bool check_valid() const { return is_valid; }

	void on_update(float delta);
	void on_render();
};