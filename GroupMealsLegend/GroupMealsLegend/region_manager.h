#pragma once
#include "region.h"
#include <string>
#include <unordered_map>

class RegionMgr
{
private:
	static RegionMgr* manager;
	std::unordered_map<std::string, Region*> region_pool;

private:
	RegionMgr() = default;
	~RegionMgr() = default;

public:
	static RegionMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void add(const std::string& name, Region* region);
	Region* find(const std::string& name);
};