#pragma once
#include "timer.h"
#include "animation.h"
#include "particle_manager.h"

// ������Ч����
class Particle
{
private:
	Timer timer_duration;							// ����������������
	Vector2 position;								// �������ɺ�,���������������
	Animation animation_vfx_left;
	Animation animation_vfx_right;
	Animation* current_animation = nullptr;
	bool is_valid = true;							// �����Ƿ���Ч
	bool is_left = false;							// �����Ƿ�����


public:
	Particle(float duration)
	{
		timer_duration.set_wait_time(duration);
		timer_duration.set_one_shot(true);
		timer_duration.set_on_timeout([&]
			{
				is_valid = false;
			});
	}
	~Particle() = default;

	virtual void on_update(float delta)
	{
		timer_duration.on_update(delta);

		current_animation = (is_left ? &animation_vfx_left : &animation_vfx_right);
		current_animation->on_update(delta);
		current_animation->set_position(position);
	}

	virtual void on_render()
	{
		if (!is_valid)
			return;

		current_animation = (is_left ? &animation_vfx_left : &animation_vfx_right);
		current_animation->on_render();
	}

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	void set_facing_left(bool flag)
	{
		is_left = flag;
	}

	bool check_valid()
	{
		return is_valid;
	}
};
