#include "character_manager.h"
#include "player.h"

CharacterManager* CharacterManager::manager = nullptr;

CharacterManager::CharacterManager()
{
	player = new Player();

}
CharacterManager::~CharacterManager() = default;


CharacterManager* CharacterManager::instance()
{
	if (nullptr == manager)
		manager = new CharacterManager;

	return manager;
}

void CharacterManager::on_input(const ExMessage& msg)
{
	player->on_input(msg);
}

void CharacterManager::on_update(float delta)
{
	player->on_update(delta);
}

void CharacterManager::on_render()
{
	player->on_render();
}

Character* CharacterManager::get_player()
{
	return player;
}

Character* CharacterManager::get_enemy()
{
	return enemy;
}