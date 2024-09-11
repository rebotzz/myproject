#pragma once
#include <vector>
#include <memory>
#include "particle_effect.h"

// 粒子特效管理器
class ParticleManager
{
private:
	static ParticleManager* manager;
	std::vector<std::shared_ptr<ParticleEffect>> particle_list;

private:
	ParticleManager() = default;
	~ParticleManager() = default;

public:
	static ParticleManager* instance();
	void on_update(float delta);
	void on_render();

	void register_particle(std::shared_ptr<ParticleEffect> particle)
	{
		particle_list.push_back(particle);
	}

};