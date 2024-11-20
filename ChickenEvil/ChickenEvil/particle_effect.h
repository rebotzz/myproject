#pragma once
#include "resources_manager.h"
#include "animation.h"

class Particle
{
protected:
	Animation anim;			// 粒子的动画
	bool valid = true;		// 粒子是否有效(生命周期随动画)

public:
	Particle()
	{
		anim.set_loop(false);
		anim.set_on_finished([&]()
			{
				valid = false;
			});
	}

	void set_position(const Vector2& position)
	{
		anim.set_position(position);
	}

	void on_update(float delta)
	{
		anim.on_update(delta);
	}

	void on_render(SDL_Renderer* renderer, const Camera& camera)
	{
		anim.on_render(renderer, camera);
	}

	bool is_valid() const
	{
		return valid;
	}

};


class ParticleEffExplode : public Particle
{
public:
	ParticleEffExplode(const Vector2& position)
	{
		anim.add_frame(ResourcesManager::instance()->find_atlas("explosion"));
		anim.set_position(position);
		anim.set_interval(0.15);
	}
};