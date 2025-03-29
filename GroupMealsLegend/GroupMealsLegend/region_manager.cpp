#include "region_manager.h"

RegionMgr* RegionMgr::manager = nullptr;
RegionMgr* RegionMgr::instance()
{
	if (manager == nullptr)
	{
		manager = new RegionMgr();
	}
	return manager;
}

void RegionMgr::on_input(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			SDL_Point point = { event.motion.x, event.motion.y };
			for (auto& iter : region_pool)
			{
				if (SDL_PointInRect(&point, &iter.second->get_rect()))
					iter.second->on_cursor_down();
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			SDL_Point point = { event.motion.x, event.motion.y };
			for (auto& [name, region] : region_pool)
			{
				if (SDL_PointInRect(&point, &region->get_rect()))
					region->on_cursor_up();
			}
		}
		break;
	default:
		break;
	}
}

void RegionMgr::on_update(float delta)
{
	for (auto& [name, region] : region_pool)
	{
		if (nullptr != region)
			region->on_update(delta);
	}
}

void RegionMgr::on_render(SDL_Renderer* renderer)
{
	for (auto& [name, region] : region_pool)
	{
		if (nullptr != region)
			region->on_render(renderer);
	}
}

void RegionMgr::on_debug_render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (auto& [name, region] : region_pool)
	{
		if (nullptr != region)
		{
			SDL_RenderDrawRect(renderer, &region->get_rect());
		}
	}
}

void RegionMgr::add(const std::string& name, Region* region)
{
	region_pool[name] = region;
}

Region* RegionMgr::find(const std::string& name)
{
	return region_pool[name];
}