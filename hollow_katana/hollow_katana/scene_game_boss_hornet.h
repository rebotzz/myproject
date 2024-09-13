#pragma once
#include <memory>
#include "scene.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "enemy_hornet.h"


class SceneGameBossHornet : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	Vector2 pos_hornet = { 1050, 620 };
	GameScene game_scene;


public:
	SceneGameBossHornet()
		:game_scene(ResourcesManager::instance()->find_image("background_hollow"), _T("bgm_1"))
	{}

	void on_input(const ExMessage& msg)
	{
		game_scene.on_input(msg);
	}
	void on_enter() override
	{
		game_scene.on_enter();

		// 改为智能指针,避免不能正确调用析构
		std::shared_ptr<EnemyHornet> hornet(new EnemyHornet);
		CharacterManager::instance()->create_enemy("hornet", hornet);
		CharacterManager::instance()->get_enemy("hornet")->set_position(pos_hornet);
		CharacterManager::instance()->get_player()->set_position(pos_player);
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
		CharacterManager::instance()->destroy_enemy("hornet");
		game_scene.on_exit();
	}
};
