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
	// ����1:����ȫ������		todo:����֮����԰������
	//scene_pool["menu_scene"] = std::shared_ptr<Scene>(new SceneMenu);
	//scene_pool["menu_scene_introduce"] = std::shared_ptr<Scene>(new SceneMenuInctroduce);
	//scene_pool["begin_scene"] = std::shared_ptr<Scene>(new SceneBegin);
	//scene_pool["transition_scene"] = std::shared_ptr<Scene>(new SceneTransition);

	//scene_pool["game_reverse_time"] = std::shared_ptr<SceneGameReverseTime>(new SceneGameReverseTime);
	//scene_pool["game_scene_boss_hornet"] = std::shared_ptr<SceneGameBossHornet>(new SceneGameBossHornet);
	//scene_pool["game_scene_boss_dragon_king"] = std::shared_ptr<SceneGameBossDragonKing>(new SceneGameBossDragonKing);
	//scene_pool["game_scene_choice"] = std::shared_ptr<SceneGameChoice>(new SceneGameChoice);

	// ����2����̬����,�������Ҳ��̫�á�todo��������Ӳ���룬����ͨ����ȡ�����ļ����ء�������ʹ�þ�����̻���
	scene_pool["transition_scene"] = std::shared_ptr<Scene>(new SceneTransition);	// ���س���Ƶ���õ�������һֱ����,�Ͳ�����ڻ�����
	scene_loader["menu_scene"] = [this]()
		{
			if (!scene_pool.count("menu_scene")) scene_pool["menu_scene"] = std::shared_ptr<Scene>(new SceneMenu);	
		};
	scene_loader["menu_scene_introduce"] = [this]()
		{
			if (!scene_pool.count("menu_scene_introduce")) scene_pool["menu_scene_introduce"] = std::shared_ptr<Scene>(new SceneMenuInctroduce);
		};
	scene_loader["begin_scene"] = [this]()
		{
			if (!scene_pool.count("begin_scene")) scene_pool["begin_scene"] = std::shared_ptr<Scene>(new SceneBegin);
		};
	scene_loader["transition_scene"] = [this]()
		{
			if (!scene_pool.count("transition_scene")) scene_pool["transition_scene"] = std::shared_ptr<Scene>(new SceneTransition);
		};
	
	scene_loader["game_reverse_time"] = [this]()
		{
			if (!scene_pool.count("game_reverse_time")) scene_pool["game_reverse_time"] = std::shared_ptr<Scene>(new SceneGameReverseTime);
		};
	scene_loader["game_scene_boss_hornet"] = [this]()
		{
			if (!scene_pool.count("game_scene_boss_hornet")) scene_pool["game_scene_boss_hornet"] = std::shared_ptr<Scene>(new SceneGameBossHornet);
		};
	scene_loader["game_scene_boss_dragon_king"] = [this]()
		{
			if (!scene_pool.count("game_scene_boss_dragon_king")) scene_pool["game_scene_boss_dragon_king"] = std::shared_ptr<Scene>(new SceneGameBossDragonKing);
		};
	scene_loader["game_scene_choice"] = [this]()
		{
			if (!scene_pool.count("game_scene_choice")) scene_pool["game_scene_choice"] = std::shared_ptr<Scene>(new SceneGameChoice);
		};

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
	{
		if (scene_loader.count(id))
		{
			scene_loader[id]();
			scene_cache.push_back(id);
		}
		else
		{
			MessageBox(GetHWnd(), _T("scene id not find"), _T("Error"), MB_OK | MB_ICONERROR);
		}
	}

	current_scene = scene_pool[id];
	cur_scene_id = id;
}

// debug: ���������, С�� switch_to_pre_scene �Լ����Լ���ֵ
void SceneManager::switch_scene(std::string id)
{
	if (scene_pool.count(id) == 0)
	{
		if (scene_loader.count(id))
		{
			scene_loader[id]();

			cout << "���س�����" << id << endl;
		}
		else
		{
			MessageBox(GetHWnd(), _T("scene id not find"), _T("Error"), MB_OK | MB_ICONERROR);
		}
	}

	cout << "�л�������" << id << endl;

	// ���³������棬����������
	auto scene_iter = std::find(scene_cache.begin(), scene_cache.end(), id);
	if (scene_iter == scene_cache.end())
	{
		scene_cache.push_back(id);
	}
	else
	{
		// ����λ��
		scene_iter = scene_cache.erase(scene_iter);
		scene_cache.push_back(id);
	}
	if (scene_cache.size() > SCENE_CACHE_SIZE)
	{
		for (auto iter = scene_cache.begin(); iter != scene_cache.end(); )
		{
			auto last_cache_id = *iter;
			if (last_cache_id != cur_scene_id && last_cache_id != prev_scene_id && last_cache_id != id)
			{
				scene_pool[last_cache_id]->on_exit();
				scene_pool.erase(last_cache_id);
				iter = scene_cache.erase(iter);

				cout << "���������棺" << last_cache_id << endl;
			}
			else
			{
				++iter;
			}

			if (scene_cache.size() <= SCENE_CACHE_SIZE)
			{
				break;
			}
		}

	}

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
	if (!scene_pool.count(id))
	{
		if (scene_loader.count(id))
		{
			scene_loader[id]();
			scene_cache.push_back(id);
		}
	}

	if (scene_pool.count(id))
	{

		SceneTransition* transition = dynamic_cast<SceneTransition*>(scene_pool["transition_scene"].get());
		transition->set_next_scene(id);
	}
}

