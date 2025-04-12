#include "region_manager.h"

#include <algorithm>

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
				if (iter.second->get_valid() && SDL_PointInRect(&point, &iter.second->get_rect()))
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
				if (region->get_valid() && SDL_PointInRect(&point, &region->get_rect()))
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
		if (nullptr != region && region->get_valid())
			region->on_update(delta);
	}
}

void RegionMgr::on_render(SDL_Renderer* renderer)
{
	for (auto& [layer, region] : render_layer_region)
	{
		if (nullptr != region && region->get_valid())
			region->on_render(renderer);
	}
}

void RegionMgr::on_debug_render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (auto& [name, region] : region_pool)
	{
		if (nullptr != region && region->get_valid())
		{
			SDL_RenderDrawRect(renderer, &region->get_rect());
		}
	}
}

void RegionMgr::add(const std::string& name, Region* region, int layer)
{
	region_pool[name] = region;
	render_layer_region.push_back(std::make_pair(layer, region));
	std::sort(render_layer_region.begin(), render_layer_region.end());
}

void RegionMgr::remove(const std::string& name)
{
	if (region_pool.count(name))
	{
		Region* target = region_pool[name];
		region_pool.erase(name);
		for (auto iter = render_layer_region.begin(); iter != render_layer_region.end(); ++iter)
		{
			if (iter->second == target)
			{
				iter = render_layer_region.erase(iter);
				break;
			}
		}
	}
}

Region* RegionMgr::find(const std::string& name)
{
	return region_pool[name];
}

void RegionMgr::set_layer(const std::string& id, unsigned int val)
{
	if (region_pool.count(id) == 0) return;
	const Region* target = region_pool[id];
	for (auto iter = render_layer_region.begin(); iter != render_layer_region.end(); ++iter)
	{
		if (iter->second == target)
		{
			render_layer_region.erase(iter);
			break;
		}
	}
	render_layer_region.push_back(std::make_pair(val, region_pool[id]));
	std::sort(render_layer_region.begin(), render_layer_region.end());		// ÉıĞò
}
