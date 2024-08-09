#pragma once
#include <memory>
#include "scene.h"

extern std::shared_ptr<Scene> menu_scene;
extern std::shared_ptr<Scene> selector_scene;
extern std::shared_ptr<Scene> game_scene;

class SceneManager
{
public:
	enum class SceneType
	{
		MENU = 0,
		GAME,
		SELECTOR
	};

private:
	Scene* _current_scene;

public:
	void set_current_scene(Scene* scene)
	{
		_current_scene = scene;
		_current_scene->on_enter();
	}

	void switch_scene(SceneType scene_type)
	{
		_current_scene->on_exit();
		switch (scene_type)
		{
		case SceneType::MENU: 
			_current_scene = menu_scene.get();
			break;
		case SceneType::SELECTOR:
			_current_scene = selector_scene.get();
			break;
		case SceneType::GAME:
			_current_scene = game_scene.get();
			break;
		default:
			break;
		}
		_current_scene->on_enter();
	}

	void on_input(ExMessage& msg)
	{
		_current_scene->on_input(msg);
	}

	void on_update(int delta_time)
	{
		_current_scene->on_update(delta_time);
	}

	void on_draw(const Camera& camera)
	{
		_current_scene->on_draw(camera);
	}
};


