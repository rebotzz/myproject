#include <stdexcept>
#include "scene_manager.h"


//#include "scene_menu.h"
//#include "scene_menu_introduce.h"
//#include "scene_game_reverse_time.h"
//#include "scene_game_boss_dragon_king.h"
//#include "scene_game_street.h"


#include "scene_game_boss_hornet.h"



SceneManager* SceneManager::manager = nullptr;

SceneManager::SceneManager()
{
	// 方案1:加载全部场景		todo:或许之后可以按需加载
	//scene_pool["menu_scene"] = std::shared_ptr<Scene>(new SceneMenu);
	//scene_pool["menu_scene_introduce"] = std::shared_ptr<Scene>(new SceneMunuInctroduce);
	//scene_pool["game_reverse_time"] = std::shared_ptr<Scene>(new SceneGameReverseTime);
	//scene_pool["game_scene_boss_dragon_king"] = std::shared_ptr<Scene>(new SceneGameBossDragonKing);
	//scene_pool["game_scene_street"] = std::shared_ptr<Scene>(new SceneGameStreet);


	scene_pool["game_scene_boss_hornet"] = std::shared_ptr<Scene>(new SceneGameBossHornet);

}

SceneManager::~SceneManager()
{}

SceneManager* SceneManager::instance()
{
	if (nullptr == manager)
		manager = new SceneManager;

	return manager;
}

void SceneManager::set_entry_scene(const std::string& id)
{
	if (scene_pool.count(id) == 0)
		throw std::invalid_argument("scene id not find");

	current_scene = scene_pool[id];
}

void SceneManager::switch_scene(const std::string& id)
{
	if (scene_pool.count(id) == 0)
		throw std::invalid_argument("scene id not find");

	current_scene->on_exit();
	current_scene = scene_pool[id];
	current_scene->on_enter();
}

void SceneManager::register_scene(const std::string& id, const std::shared_ptr<Scene>& scene)
{
	scene_pool[id] = scene;
}



void SceneManager::on_update(int delta_time)
{
	if (need_init)
	{
		current_scene->on_enter();
		need_init = false;
	}

	current_scene->on_update(delta_time);
}