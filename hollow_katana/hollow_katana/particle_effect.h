#pragma once
#include "timer.h"
#include "animation.h"

// 粒子特效基类
class ParticleEffect
{
protected:
	// 管理粒子生命周期, 粒子生命周期随动画,动画结束,粒子就结束了
	Vector2 position;								// 粒子生成后,世界坐标基本不变
	Animation animation_vfx;
	Animation* current_animation = nullptr;
	bool is_valid = true;							// 粒子是否有效

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
