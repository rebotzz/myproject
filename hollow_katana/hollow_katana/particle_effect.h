#pragma once
#include "timer.h"
#include "animation.h"

// ������Ч����
class ParticleEffect
{
protected:
	// ����������������, �������������涯��,��������,���Ӿͽ�����
	Vector2 position;								// �������ɺ�,���������������
	Animation animation_vfx;
	Animation* current_animation = nullptr;
	bool is_valid = true;							// �����Ƿ���Ч

public:
	ParticleEffect()
	{
		animation_vfx.set_loop(false);
		animation_vfx.set_on_finished([&] { is_valid = false; });
	}
	~ParticleEffect() = default;

	virtual void on_update(float delta)
	{
		animation_vfx.on_update(delta);
		animation_vfx.set_position(position);
	}

	virtual void on_render()
	{
		if (!is_valid)
			return;

		animation_vfx.on_render();
	}

	void set_position(const Vector2& target_position)
	{
		position = target_position;
	}

	bool check_valid() const
	{
		return is_valid;
	}
};
