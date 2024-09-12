#include <stdexcept>
#include "character_manager.h"
#include "bullet_time_manager.h"
#include "particle_manager.h"
#include "player.h"
#include "enemy_hornet.h"


CharacterManager* CharacterManager::manager = nullptr;

CharacterManager::CharacterManager()
{
	player = std::shared_ptr<Character>(new Player);
}
CharacterManager::~CharacterManager()
{}

CharacterManager* CharacterManager::instance()
{
	if (nullptr == manager)
		manager = new CharacterManager();

	return manager;
}

void CharacterManager::on_input(const ExMessage& msg)
{
	if(is_enable)
		player->on_input(msg);
}

void CharacterManager::on_update(float delta)
{
	if (is_enable)
	{
		for (auto& kv : enemy_list)
			kv.second->on_update(delta);

		player->on_update(delta);
	}
}

void CharacterManager::on_render()
{
	if (is_enable)
	{
		for (auto& kv : enemy_list)
			kv.second->on_render();

		BulletTimeManager::instance()->post_progress();

		player->on_render();
	}
}

Character* CharacterManager::get_player()
{
	return player.get();
}

Character* CharacterManager::get_enemy()
{
	if (enemy_list.empty())
		throw std::invalid_argument("enemy_list is empty.");

	auto first_enemy = enemy_list.begin();
	Character* enemy = first_enemy->second.get();

	return enemy;
}

Character* CharacterManager::get_enemy(const std::string& id)
{
	if(enemy_list.count(id) == 0)
		throw std::invalid_argument("enemy id not exist.");

	return enemy_list[id].get();
}

void CharacterManager::create_enemy(const std::string& id, std::shared_ptr<Character> enemy)
{
	enemy_list[id] = enemy;
}

void CharacterManager::destroy_enemy(const std::string& id)
{
	auto iterator = enemy_list.find(id);
	if (iterator != enemy_list.end())
		enemy_list.erase(iterator);
}