#include "character_manager.h"
#include "player.h"
#include "bullet_time_manager.h"
#include "enemy_hornet.h"

CharacterManager* CharacterManager::manager = nullptr;

CharacterManager::CharacterManager()
{
	enemy = new EnemyHornet;
	player = new Player;
}
CharacterManager::~CharacterManager()
{
	delete player;
	delete enemy;
}

CharacterManager* CharacterManager::instance()
{
	if (nullptr == manager)
		manager = new CharacterManager();

	return manager;
}

void CharacterManager::on_input(const ExMessage& msg)
{
	player->on_input(msg);
}

void CharacterManager::on_update(float delta)
{
	enemy->on_update(delta);
	player->on_update(delta);
}

void CharacterManager::on_render()
{
	enemy->on_render();
	BulletTimeManager::instance()->post_progress();
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