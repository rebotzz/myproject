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
	
	// �����Ч����		debug: ��������ָ�����,�������ڴ�й¶,���ʳ���
	particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(),
		[](std::shared_ptr<ParticleEffect>& particle)
		{
			bool can_remove = !particle->check_valid();
			return can_remove;
		}), particle_list.end());
}
void  ParticleManager::on_render()
{
	// ��Ⱦ����
	for (std::shared_ptr<ParticleEffect>& particle : particle_list)
	{
		if (particle->check_valid())
			particle->on_render();
	}
}