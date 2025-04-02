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
	for (auto& [name, region] : region_pool)
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
	render_layer.push_back(std::make_pair(layer, region));
	std::sort(render_layer.begin(), render_layer.end(), [](std::pair<int, Region*>& p1, std::pair<int, Region*>& p2)
		{
			return p1.first < p2.first;
		});
}

Region* RegionMgr::find(const std::string& name)
{
	return region_pool[name];
}

void RegionMgr::set_layer(const std::string& id, unsigned int val)
{
	if (region_pool.count(id) == 0) return;
	const Region* target = region_pool[id];
	for (auto iter = render_layer.begin(); iter != render_layer.end(); ++iter)
	{
		if (iter->second == target)
		{
			render_layer.erase(iter);
			break;
		}
	}
	render_layer.push_back(std::make_pair(val, region_pool[id]));
	std::sort(render_layer.begin(), render_layer.end());		// ÉıĞò
}
