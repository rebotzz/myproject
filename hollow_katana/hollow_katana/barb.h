#pragma once
#include "animation.h"
#include "vector2.h"
#include "timer.h"
#include "collision_box.h"

class Barb
{
private:
	enum class Status
	{
		Idle, Aim, Dash, Break
	};

private:
	const float SPEED_DASH = 1500.0f;

private:
	Timer timer_idle;							// ����״̬ʱ��
	Timer timer_aim;							// ��׼״̬ʱ��
	float total_delta_time = 0.0f;				// ���ɺ�ȹ���ʱ��
	float diff_period = 0.0f;					// ���ֵ,���и�������ƫ��
	bool is_valid = true;

	Vector2 velocity;
	Vector2 base_position;
	Vector2 current_position;

	Animation animation_loose;
	Animation animation_break;
	Animation* current_animation = nullptr;
	CollisionBox* collision_box = nullptr;
	Status status = Status::Idle;

public:
	Barb(const Vector2& position);
	~Barb();

	void on_update(float delta);
	void on_render();

	void set_position(const Vector2& position)
	{
		this->current_position = position;
	}

	bool check_valid() const
	{
		return is_valid;
	}

private:
	void on_break();

};