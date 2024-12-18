#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "scene.h"


// µ¥Àý
class SceneManager
{
private:
	static SceneManager* manager;
	std::shared_ptr<Scene> current_scene = nullptr;
	std::unordered_map<std::string, std::shared_ptr<Scene>> scene_pool;

private:
	SceneManager() {};
	SceneManager(const SceneManager&) = delete;

public:
	static SceneManager* instance();

	virtual void on_input(const SDL_Event& event)
	{
		current_scene->on_input(event);
	}

	virtual void on_update(double delta)
	{
		current_scene->on_update(delta);
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		current_scene->on_render(renderer);
	}

	void switch_scene(const std::string& id)
	{
		if (scene_pool.count(id) == 0)
			return;

		if(current_scene)
			current_scene->on_exit();
		current_scene = scene_pool[id];
		current_scene->on_enter();
	}


	bool register_scene(const std::string& id, const std::shared_ptr<Scene>& scene)
	{
		if (scene_pool.count(id) == 0 && nullptr != scene)
		{
			scene_pool[id] = scene;
			return true;
		}
		else
			return false;
	}

	void remove_scene(const std::string& id)
	{
		if (scene_pool.count(id))
		{
			scene_pool.erase(id);
		}
	}
};