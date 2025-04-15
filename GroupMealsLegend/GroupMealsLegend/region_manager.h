#pragma once
#include "region.h"

#include <string>
#include <unordered_map>
#include <vector>

class RegionMgr
{
private:
	static RegionMgr* manager;
	std::unordered_map<std::string, Region*> region_pool;			// 区域池
	std::vector<std::pair<int, Region*>> render_layer_region;		// 区域渲染分层
	bool need_sort = false;											// 是否需要排序，保证分层渲染

private:
	RegionMgr() = default;
	~RegionMgr() = default;

public:
	static RegionMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);
	void on_debug_render(SDL_Renderer* renderer);

	void add(const std::string& name, Region* region, int layer = 0);
	void remove(const std::string& name);
	Region* find(const std::string& name);
	void set_layer(const std::string& id, unsigned int val);
};