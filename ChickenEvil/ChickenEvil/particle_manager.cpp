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
	// ��������
	for (auto& particle : particle_list)
		particle->on_update(delta);
	
	// �����Ч����	
	particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(),
		[](std::shared_ptr<Particle>& particle)
		{
			return !particle->is_valid();
		}), particle_list.end());
}
void  ParticleManager::on_render(const Camera& camera) const
{
	// ��Ⱦ����
	for (const std::shared_ptr<Particle>& particle : particle_list)
	{
		if (particle->is_valid())
			particle->on_render(camera);
	}
}