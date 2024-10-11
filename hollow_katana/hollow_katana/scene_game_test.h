#pragma once
#include <memory>
#include "scene.h"
#include "scene_game.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "particle_manager.h"
#include "audio_manager.h"


// todo: 
// 1.增加挡板,防止角色飞出屏幕
// 2.重力更改时,调整动画方向,碰撞箱体方向,锚点位置,或许还有跳跃方向
// 3.上平台地面
// 
//
class SceneGameTest : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	Vector2 pos_door = { 1150, 620 };
	Vector2 pos_wooden_man = { 800, 620 };
	GameScene game_scene;
	Direction dir_gravity = Direction::Down;

public:
	SceneGameTest()
		:game_scene(ResourcesManager::instance()->find_image("background_katana"), _T("bgm_0"))
	{
	}

	void on_enter() override
	{
		game_scene.on_enter();
		game_scene.stop_record();

		CharacterManager::instance()->get_player()->set_position(pos_player);
	}
	void on_input(const ExMessage& msg)
	{
		game_scene.on_input(msg);
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case VK_UP:
				CharacterManager::instance()->get_player()->set_gravity_dir(Direction::Up);
				break;
			case VK_DOWN:
				CharacterManager::instance()->get_player()->set_gravity_dir(Direction::Down);
				break;
			case VK_LEFT:
				CharacterManager::instance()->get_player()->set_gravity_dir(Direction::Left);
				break;
			case VK_RIGHT:
				CharacterManager::instance()->get_player()->set_gravity_dir(Direction::Right);
				break;
			}
			break;
		}
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
		game_scene.on_exit();
	}
};
