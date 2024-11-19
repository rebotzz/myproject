//#include "particle_manager.h"
//
//
//
//ParticleManager* ParticleManager::manager = nullptr;
//ParticleManager* ParticleManager::instance()
//{
//	if (nullptr == manager)
//		manager = new ParticleManager;
//
//	return manager;
//}
//void  ParticleManager::on_update(float delta)
//{
//	// 更新粒子
//	for (auto& particle : particle_list)
//		particle->on_update(delta);
//	
//	// 清除无效粒子		debug: 还是智能指针好用,不容易内存泄露,访问出错
//	particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(),
//		[](std::shared_ptr<ParticleEffect>& particle)
//		{
//			bool can_remove = !particle->check_valid();
//			return can_remove;
//		}), particle_list.end());
//}
//void  ParticleManager::on_render()
//{
//	// 渲染粒子
//	for (std::shared_ptr<ParticleEffect>& particle : particle_list)
//	{
//		if (particle->check_valid())
//			particle->on_render();
//	}
//}