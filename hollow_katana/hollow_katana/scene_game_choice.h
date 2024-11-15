#pragma once
#include <memory>
#include "scene.h"
#include "scene_game.h"
#include "resources_manager.h"
#include "door.h"
#include "woodenman.h"

// v1.3新增:传送门,木头人

// todo: 
// 1.增加挡板,防止角色飞出屏幕
// 2.重力更改时,调整动画方向,碰撞箱体方向,锚点位置,或许还有跳跃方向(暂时废弃,或许在别的游戏中实现)
// 3.上平台地面

class SceneGameChoice : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	Vector2 pos_wooden_man = { 800, 620 };
	GameScene game_scene;

	Door* door_hornet = nullptr;
	Door* door_dragon_king = nullptr;
	Woodenman* woodenman = nullptr;

public:
	SceneGameChoice()
		:game_scene(ResourcesManager::instance()->find_image("background_katana"), _T("bgm_0"))
	{
		game_scene.stop_record();
	}

	void on_enter() override
	{
		game_scene.on_enter();
		game_scene.stop_record();
		CharacterManager::instance()->get_player()->set_position(pos_player);

		door_hornet = new Door("game_scene_boss_hornet");
		door_hornet->set_describe(_T("boss大黄蜂"));
		door_hornet->set_size({ 60, 120 });
		door_hornet->set_position({ 870, 560 });
		door_hornet->set_color(RGB(255, 50, 50));

		door_dragon_king = new Door("game_scene_boss_dragon_king");
		door_dragon_king->set_describe(_T("boss龙王"));
		door_dragon_king->set_size({ 60, 120 });
		door_dragon_king->set_position({ 1190, 560 });
		door_dragon_king->set_color(RGB(255, 255, 0));

		woodenman = new Woodenman;
		woodenman->set_position({ 550.0f, 620 - woodenman->get_img_size().y / 2});
	}
	void on_input(const ExMessage& msg)
	{
		game_scene.on_input(msg);
	}
	void on_update(float delta) override
	{
		game_scene.on_update(delta);
		door_hornet->on_update(delta);
		door_dragon_king->on_update(delta);
	}
	void on_render() override
	{
		//game_scene.on_render();
		game_scene.render_background();

		door_hornet->on_render();
		door_dragon_king->on_render();
		woodenman->on_render();

		CharacterManager::instance()->on_render();
		ParticleManager::instance()->on_render();
	}
	void on_exit() override
	{
		game_scene.clear_record();
		game_scene.on_exit();

		if (door_hornet) delete door_hornet;
		if (door_dragon_king) delete door_dragon_king;
		if (woodenman) delete woodenman;
	}
};
