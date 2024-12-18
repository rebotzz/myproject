#include "scene_manager.h"

SceneManager* SceneManager::manager = nullptr;
SceneManager* SceneManager::instance()
{
	if (nullptr == manager)
		manager = new SceneManager;

	return manager;
}