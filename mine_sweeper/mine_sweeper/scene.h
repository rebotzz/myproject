#pragma once
#include <easyx.h>

extern int grid_x;
extern int grid_y;

class Scene
{
public:
	virtual void on_enter()
	{}

	virtual void on_exit()
	{}

	virtual void on_input(const ExMessage& msg)
	{}

	virtual void on_update(int delta)
	{}

	virtual void on_draw()
	{}
};


class MenuScene : public Scene
{
private:
	enum class Mode
	{
		Simple,
		Middle,
		Expert,
		FullSreen
	};
private:

public:
	virtual void on_enter()
	{}

	virtual void on_exit()
	{}

	virtual void on_input(const ExMessage& msg)
	{}

	virtual void on_update(int delta)
	{}

	virtual void on_draw()
	{}
};


class GameScene : public Scene
{
private:

public:
	virtual void on_enter()
	{}

	virtual void on_exit()
	{}

	virtual void on_input(const ExMessage& msg)
	{}

	virtual void on_update(int delta)
	{}

	virtual void on_draw()
	{}
};


extern MenuScene menu_scene;
extern GameScene game_scene;

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game
	};
private:
	Scene* current_scene = nullptr;
public:

	virtual void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}

	virtual void on_update(int delta)
	{
		current_scene->on_update(delta);
	}

	virtual void on_draw()
	{
		current_scene->on_draw();
	}

	void switch_scene(SceneType type)
	{
		current_scene->on_exit();
		switch (type)
		{
		case SceneType::Menu:
			current_scene = &menu_scene;
			break;
		case SceneType::Game:
			current_scene = &game_scene;
			break;
		}
		current_scene->on_enter();
	}
};