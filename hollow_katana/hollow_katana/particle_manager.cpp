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
	for (Particle* particle : particle_list)
		particle->on_update(delta);
	
	// �����Ч����
	particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(),
		[](Particle* particle)
		{
			bool can_remove = !particle->check_valid();
			if (can_remove) delete particle;
			return can_remove;
		}), particle_list.end());
}
void  ParticleManager::on_render()
{
	// ��Ⱦ����
	for (Particle* particle : particle_list)
		particle->on_render();
}