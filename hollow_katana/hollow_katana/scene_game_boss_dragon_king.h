#pragma once
#include <memory>
#include "scene.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "particle_manager.h"
#include "reverse_time_manager.h"
#include "audio_manager.h"
#include "bullet_time_manager.h"
#include "enemy_dragon_king.h"

class SceneGameBossDragonKing : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	Vector2 pos_enemy = { 1050, 620 };
	GameScene game_scene;

public:
	SceneGameBossDragonKing()
		:game_scene(ResourcesManager::instance()->find_image("background_katana"), _T("bgm_1"))
	{}

	void on_enter() override
	{
		game_scene.on_enter();

		std::shared_ptr<EnemyDragonKing> dragon_king(new EnemyDragonKing);
		CharacterManager::instance()->create_enemy("dragon_king", dragon_king);
		CharacterManager::instance()->get_enemy("dragon_king")->set_position(pos_enemy);
		CharacterManager::instance()->get_player()->set_position(pos_player);
		cout << "½øÈë: SceneGameBossDragonKing" << endl;
	}
	void on_input(const ExMessage& msg)
	{
		game_scene.on_input(msg);
	}
	void on_update(float delta) override
	{
		game_scene.on_update(delta);
	}
	void on_render() override
	{
		game_scene.on_render();
	}
	void on_exit() override
	{
		CharacterManager::instance()->destroy_enemy("dragon_king");
		game_scene.on_exit();
	}

};
