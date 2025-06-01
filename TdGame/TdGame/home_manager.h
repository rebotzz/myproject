#pragma once

#include "singleton.h"
#include "observer.h"
#include "config_manager.h"
#include "collision_manager.h"

class HomeMgr : public Singleton<HomeMgr>, public Subject
{
	friend class Singleton<HomeMgr>;
public:

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Texture* tex_home = ResMgr::instance()->find_texture(ResID::Tex_Home);
		static SDL_Rect rect_home = { ConfigMgr::instance()->home.x * TILE_SIZE,
			ConfigMgr::instance()->home.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
		SDL_RenderCopy(renderer, tex_home, nullptr, &rect_home);
	}

	int get_hp() const
	{
		return cur_hp;
	}

	int get_tower_upgrade_cost(TowerType type) const
	{
		int cost = 0;
		ConfigMgr* config = ConfigMgr::instance();
		switch (type)
		{
		case TowerType::Archer:
			cost = archer_level >= 9 ? -1 : config->tower_archer_template.upgrade_cost[archer_level];
			break;
		case TowerType::Axeman:
			cost = archer_level >= 9 ? -1 : config->tower_axeman_template.upgrade_cost[axeman_level];
			break;
		case TowerType::Gunner:
			cost = archer_level >= 9 ? -1 : config->tower_gunner_template.upgrade_cost[gunner_level];
			break;
		}
		return cost;
	}

	int get_tower_place_cost(TowerType type) const
	{
		int cost = 0;
		ConfigMgr* config = ConfigMgr::instance();
		switch (type)
		{
		case TowerType::Archer:
			cost = config->tower_archer_template.cost[archer_level];
			break;
		case TowerType::Axeman:
			cost = config->tower_axeman_template.cost[axeman_level];
			break;
		case TowerType::Gunner:
			cost = config->tower_gunner_template.cost[gunner_level];
			break;
		}
		return cost;
	}

	void upgrade_tower(TowerType type)
	{
		switch (type)
		{
		case TowerType::Archer:
			archer_level = std::min(9, archer_level + 1);
			break;
		case TowerType::Axeman:
			axeman_level = std::min(9, axeman_level + 1);
			break;
		case TowerType::Gunner:
			gunner_level = std::min(9, gunner_level + 1);
			break;
		}

		Subject::notify_observer();
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_TowerLevelUp), 0);
	}

	int get_tower_level(TowerType type) const
	{
		int level = 0;
		switch (type)
		{
		case TowerType::Archer:
			level = archer_level;
			break;
		case TowerType::Axeman:
			level = axeman_level;
			break;
		case TowerType::Gunner:
			level = gunner_level;
			break;
		}
		return level;
	}


protected:
	HomeMgr()
	{
		home_box = CollisionMgr::instance()->create_collision_box();
		home_box->set_src_layer(CollisionLayer::Home);
		home_box->set_size({ TILE_SIZE / 2, TILE_SIZE / 2});
		home_box->set_on_collide([this]()
			{
				Enemy* enemy = reinterpret_cast<Enemy*>(home_box->get_on_collide_box()->get_user_data());
				if (!enemy || enemy->can_remove()) return;
				cur_hp -= enemy->get_damage();
				enemy->make_invalid();
				if (cur_hp < 0) cur_hp = 0;
				Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_HomeHurt), 0);
				if (cur_hp == 0) Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_Loss), 0);
			});
		Vector2 pos_home = { (float)(ConfigMgr::instance()->home.x * TILE_SIZE + TILE_SIZE / 2), 
		(float)(ConfigMgr::instance()->home.y * TILE_SIZE + TILE_SIZE / 2)};

		home_box->set_position(pos_home);
	}
	~HomeMgr()
	{
		CollisionMgr::instance()->destroy_collision_box(home_box);
	}

private:
	// 属性
	int init_max_hp = 10;
	int cur_hp = 10;
	int archer_level = 0;
	int axeman_level = 0;
	int gunner_level = 0;

	// 敌人进入home碰撞检测
	CollisionBox* home_box = nullptr;
};

