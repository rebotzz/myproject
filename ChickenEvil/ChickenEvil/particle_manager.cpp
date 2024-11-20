#include "particle_manager.h"

ParticleManager* ParticleManager::manager = nullptr;
ParticleManager* ParticleManager::instance()
{
	if (nullptr == manager)
		manager = new ParticleManager;

	return manager;
}
void  ParticleManager::on_update(float delta)
{
	// 更新粒子
	for (auto& particle : particle_list)
		particle->on_update(delta);
	
	// 清除无效粒子	
	particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(),
		[](std::shared_ptr<Particle>& particle)
		{
			return !particle->is_valid();
		}), particle_list.end());
}
void  ParticleManager::on_render(SDL_Renderer* renderer, const Camera& camera)
{
	// 渲染粒子
	for (std::shared_ptr<Particle>& particle : particle_list)
	{
		if (particle->is_valid())
			particle->on_render(renderer, camera);
	}
}