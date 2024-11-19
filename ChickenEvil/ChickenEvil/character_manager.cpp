#include "character_manager.h"


CharacterManager* CharacterManager::manager = nullptr;
CharacterManager* CharacterManager::instance()
{
	if (manager == nullptr)
	{
		manager = new CharacterManager;
	}
	return manager;
}

static std::shared_ptr<Chicken> spawn_chicken();

CharacterManager::CharacterManager()
{
	timer_spawn_enemy.set_one_shot(false);
	timer_spawn_enemy.set_wait_time(0.3f);
	timer_spawn_enemy.set_on_timeout([&]()
		{
			int rand_num = rand() % 3;
			for (int i = 0; i < rand_num; ++i)
				enemy_list.push_back(spawn_chicken());
		});

	turrent_list.push_back(std::shared_ptr<Turrent>(new Turrent({665, 500})));
}

void CharacterManager::on_input(SDL_Event* event)
{
	for (auto& turrent : turrent_list)
		turrent->on_input(event);
}

void CharacterManager::on_update(float delta)
{
	// 游戏结束
	if (home_hp <= 0)
	{
		SDL_Log("game over!\n");
		system("pause");
		exit(0);
	}


	timer_spawn_enemy.on_update(delta);
	for (auto& enemy : enemy_list)
	{
		enemy->on_update(delta);

		// 死亡，放烟花
		if (!enemy->is_alive())
		{

		}

		// 越界，扣血
		if (enemy->is_alive() && enemy->get_position().y > (double)WINDOW_H + 50.0f)
		{
			enemy->dead();
			home_hp--;
		}
	}

	enemy_list.erase(std::remove_if(enemy_list.begin(), enemy_list.end(), 
		[](const std::shared_ptr<Chicken>& chicken)
		{
			return !chicken->is_alive();
		}), enemy_list.end());


	for (auto& turrent : turrent_list)
	{
		turrent->on_update(delta);
	}
}

void CharacterManager::on_render(SDL_Renderer* renderer, const Camera& camera)
{
	for (auto& enemy : enemy_list)
	{
		enemy->on_render(renderer, camera);
	}

	for (auto& turrent : turrent_list)
	{
		turrent->on_render(renderer, camera);
	}
}

static std::shared_ptr<Chicken> spawn_chicken()
{
	// 随机种类
	std::shared_ptr<Chicken> chicken = nullptr;
	int rand_num = rand() % 100;
	if (rand_num <= 30)
	{
		chicken = std::shared_ptr<Chicken>(new ChickenSlow);
	}
	else if (rand_num <= 80)
	{
		chicken = std::shared_ptr<Chicken>(new ChickenMedium);
	}
	else
	{
		chicken = std::shared_ptr<Chicken>(new ChickenFast);
	}

	return chicken;
}