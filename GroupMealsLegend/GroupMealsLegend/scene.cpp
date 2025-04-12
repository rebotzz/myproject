#include "scene.h"

#include "region_manager.h"

void Scene::on_input(const SDL_Event& event)
{
	RegionMgr::instance()->on_input(event);
}

void Scene::on_update(float delta)
{
	RegionMgr::instance()->on_update(delta);
}

void Scene::on_render(SDL_Renderer* renderer)
{
	RegionMgr::instance()->on_render(renderer);
}