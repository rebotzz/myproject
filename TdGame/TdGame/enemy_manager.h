#pragma once

#include "singleton.h"
#include "enemy_slime.h"
#include "enemy_king_slime.h"
#include "enemy_skeleton.h"
#include "enemy_goblin.h"
#include "enemy_goblin_priest.h"
#include "coin_manager.h"

#include <vector>

class EnemyMgr : public Singleton<EnemyMgr>
{
	friend Singleton<EnemyMgr>;
	typedef std::vector<Enemy*> EnemyList;

public:
	void on_update(float delta)
	{
		for (Enemy* enemy : enemy_list)
			enemy->on_update(delta);

		int size = enemy_list.size();

		// 移除无效
		enemy_list.erase(std::remove_if(enemy_list.begin(), enemy_list.end(),
			[](Enemy* enemy)
			{
				bool deletable = enemy->can_remove();

				if (deletable)
				{
					// 被击杀，概率爆金币
					if (enemy->get_hp() <= 0 && (rand() % 100 < enemy->get_reward_ration() * 100))
						CoinMgr::instance()->create_coin_prop(enemy->get_position());
					delete enemy;
				}
				return deletable;
			}), enemy_list.end());
	}

	void on_renderer(SDL_Renderer* renderer)
	{
		for (Enemy* enemy : enemy_list)
			enemy->on_render(renderer);
	}

	void create_enemy(EnemyType type, int spawn_point)
	{
		Enemy* enemy = nullptr;
		switch (type)
		{
		case EnemyType::Slime:
			enemy = new EnemySlime();
			break;
		case EnemyType::KingSlime:
			enemy = new EnemyKingSlime();
			break;
		case EnemyType::Skeleton:
			enemy = new EnemySkeleton();
			break;
		case EnemyType::Goblin:
			enemy = new EnemyGoblin();
			break;
		case EnemyType::GoblinPriest:
			enemy = new EnemyGoblinPriest();
			break;
		default:
			enemy = new EnemySlime();
			break;
		}
		enemy->set_spawn_point(spawn_point);
		enemy_list.push_back(enemy);
	}

	EnemyList& get_enemy_list()
	{
		return enemy_list;
	}

protected:
	EnemyMgr() = default;
	~EnemyMgr()
	{
		for (Enemy* enemy : enemy_list)
			delete enemy;
	}

private:
	EnemyList enemy_list;
};

