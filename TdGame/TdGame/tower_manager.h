#pragma once

#include "singleton.h"
#include "tower_archer.h"
#include "tower_axeman.h"
#include "tower_gunner.h"

#include <vector>

class TowerMgr : public Singleton<TowerMgr>
{
	friend class Singleton<TowerMgr>;
	typedef std::vector<Tower*> TowerList;
public:
	void on_update(float delta)
	{
		for (Tower* tower : tower_list)
			tower->on_update(delta);
	}

	void on_renderer(SDL_Renderer* renderer)
	{
		for (Tower* tower : tower_list)
			tower->on_render(renderer);
	}

	void create_tower(TowerType type, const Vector2& position)
	{
		Tower* tower = nullptr;
		switch (type)
		{
		case TowerType::Archer:
			tower = new TowerArcher(position);
			break;
		case TowerType::Axeman:
			tower = new TowerAxeman(position);
			break;
		case TowerType::Gunner:
			tower = new TowerGunner(position);
			break;
		}
		tower_list.push_back(tower);

		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_PlaceTower), 0);
	}

protected:
	TowerMgr() = default;
	~TowerMgr()
	{
		for (Tower* tower : tower_list)
			delete tower;
	}

private:
	TowerList tower_list;
};
