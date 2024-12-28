#pragma once
#include <SDL.h>
#include <vector>
#include <memory>
#include "camera.h"
#include "particle_effect.h"

// 粒子特效管理器
class ParticleManager
{
private:
	static ParticleManager* manager;
	std::vector<std::shared_ptr<Particle>> particle_list;

private:
	ParticleManager() = default;
	~ParticleManager() = default;

public:
	static ParticleManager* instance();
	void on_update(float delta);
	void on_render(const Camera& camera) const;

	void register_particle(const std::shared_ptr<Particle>& particle)
	{
		particle_list.push_back(particle);
	}

};