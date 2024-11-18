#include <stdexcept>
#include "scene_manager.h"
#include "scene_menu.h"
#include "scene_transition.h"
#include "scene_game_boss_hornet.h"
#include "scene_game_reverse_time.h"
#include "scene_game_boss_dragon_king.h"
#include "scene_game_choice.h"

SceneManager* SceneManager::manager = nullptr;

SceneManager::SceneManager()
{
	// 方案1:加载全部场景		todo:或许之后可以按需加载
	scene_pool["menu_scene"] = std::shared_ptr<Scene>(new SceneMenu);
	scene_pool["menu_scene_introduce"] = std::shared_ptr<Scene>(new SceneMenuInctroduce);
	scene_pool["begin_scene"] = std::shared_ptr<Scene>(new SceneBegin);
	scene_pool["transition_scene"] = std::shared_ptr<Scene>(new SceneTransition);

	scene_pool["game_reverse_time"] = std::shared_ptr<SceneGameReverseTime>(new SceneGameReverseTime);
	scene_pool["game_scene_boss_hornet"] = std::shared_ptr<SceneGameBossHornet>(new SceneGameBossHornet);
	scene_pool["game_scene_boss_dragon_king"] = std::shared_ptr<SceneGameBossDragonKing>(new SceneGameBossDragonKing);
	scene_pool["game_scene_choice"] = std::shared_ptr<SceneGameChoice>(new SceneGameChoice);
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
		MessageBox(GetHWnd(), _T("scene id not find"), _T("Error"), MB_OK | MB_ICONERROR);

	current_scene = scene_pool[id];
	cur_scene_id = id;
}

// debug: 如果是引用, 小心 switch_to_pre_scene 自己给自己赋值
void SceneManager::switch_scene(std::string id)
{
	if (scene_pool.count(id) == 0)
		throw std::invalid_argument("scene id not find");

	prev_scene_id = cur_scene_id;
	cur_scene_id = id;

	current_scene->on_exit();
	current_scene = scene_pool[cur_scene_id];
	current_scene->on_enter();
}

void SceneManager::register_scene(const std::string& id, const std::shared_ptr<Scene>& scene)
{
	scene_pool[id] = scene;
}

void SceneManager::on_input(const ExMessage& msg)
{
	if (need_init)
	{
		current_scene->on_enter();
		need_init = false;
	}

	current_scene->on_input(msg);
}

void SceneManager::on_update(float delta)
{
	if (need_init)
	{
		current_scene->on_enter();
		need_init = false;
	}

	current_scene->on_update(delta);
	camera.on_update(delta);
}

void SceneManager::set_transition_next_scene(const std::string& id)
{
	if (scene_pool.count(id))
	{
		SceneTransition* transition = dynamic_cast<SceneTransition*>(scene_pool["transition_scene"].get());
		transition->set_next_scene(id);
	}
}

