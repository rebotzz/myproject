#include "scene_manager.h"

SceneMgr* SceneMgr::manager = nullptr;

SceneMgr* SceneMgr::instance()
{
	if (!manager)
	{
		manager = new SceneMgr();
	}
	return manager;
}

void SceneMgr::on_input(const SDL_Event& event)
{
	if (cur_scene)
		cur_scene->on_input(event);
}
void SceneMgr::on_update(float delta)
{
	if (cur_scene)
		cur_scene->on_update(delta);
}
void SceneMgr::on_render(SDL_Renderer* renderer)
{
	if (cur_scene)
		cur_scene->on_render(renderer);
}

void SceneMgr::add(const std::string& id, Scene* scene)
{
	scene_pool[id] = scene;
}

void SceneMgr::switch_scene(const std::string& id)
{
	if (cur_scene)
		cur_scene->on_exit();
	cur_scene = scene_pool[id];
	if (cur_scene)
		cur_scene->on_enter();
}