#pragma once
#include <memory>
#include "scene.h"
#include "scene_game.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "particle_manager.h"
#include "reverse_time_manager.h"
#include "audio_manager.h"
#include "bullet_time_manager.h"
#include "enemy_dragon_king.h"
//#include "scene_menu.h"


class SceneGameBossDragonKing : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	Vector2 pos_enemy = { 1050, 620 };
	GameScene game_scene;
	Timer timer_win;
	bool is_win = false;

public:
	SceneGameBossDragonKing()
		:game_scene(ResourcesManager::instance()->find_image("background_katana"), _T("bgm_2"))
	{
		timer_win.set_one_shot(true);
		timer_win.set_wait_time(15.f);
		timer_win.set_on_timeout([&]
			{
				game_scene.clear_record();
				SceneTransition* transition = dynamic_cast<SceneTransition*>(
					SceneManager::instance()->get_transition_scene());
				transition->set_next_scene("menu_scene");
				SceneManager::instance()->switch_scene("transition_scene");
			});
	}

	void on_enter() override
	{
		game_scene.on_enter();
		timer_win.restart();
		is_win = false;

		std::shared_ptr<EnemyDragonKing> dragon_king(new EnemyDragonKing);
		CharacterManager::instance()->create_enemy("dragon_king", dragon_king);
		CharacterManager::instance()->get_enemy("dragon_king")->set_position(pos_enemy);
		CharacterManager::instance()->get_player()->set_position(pos_player);
	}
	void on_input(const ExMessage& msg)
	{
		game_scene.on_input(msg);
	}
	void on_update(float delta) override
	{
		game_scene.on_update(delta);

		// Ê¤ÀûÌø×ª
		if (CharacterManager::instance()->get_player()->get_hp() > 0
			&& (CharacterManager::instance()->get_enemy() == nullptr
				|| CharacterManager::instance()->get_enemy()->get_hp() <= 0))
			is_win = true;

		if (is_win)
			timer_win.on_update(delta);
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
