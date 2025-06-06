#pragma once
#include <memory>
#include <vector>
#include "scene.h"
#include "scene_game.h"
#include "resources_manager.h"
#include "door.h"
#include "woodenman.h"
#include "platform.h"




// v1.3新增:传送门,木头人,平台，开关
// todo: 
// 1.增加挡板,防止角色飞出屏幕顶部
// 2.重力更改时,调整动画方向,碰撞箱体方向,锚点位置,或许还有跳跃方向(暂时废弃,或许在别的游戏中实现)
// TODO:场景最好动态加载，用到时候在创建，一股脑全创建容易产生很多碰撞箱 | 或者场景内素材进入加载，退出销毁
class SceneGameChoice : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	GameScene game_scene;

	// 交互功能道具
	Door* door_hornet = nullptr;							
	Door* door_dragon_king = nullptr;
	Woodenman* woodenman = nullptr;
	std::vector<Platform*> platform_list;			// 跳跃平台
	bool enabled_platforms = false;
	InteractProp prop_platform_switch;				// 平台开关
	Timer timer_platforms_switch;

public:
	SceneGameChoice()
		:game_scene(ResourcesManager::instance()->find_image("background_katana"), _T("bgm_0"))
	{
		// 没有战斗，停止画面记录
		game_scene.stop_record();

		prop_platform_switch.set_describe(_T("跳跃平台开关"));
		prop_platform_switch.set_on_interact([&]()
			{
				timer_platforms_switch.restart();
			});
		prop_platform_switch.set_size({ 20, 70 });
		prop_platform_switch.set_position({ 720, 580 });
		prop_platform_switch.enable_showing_box(true);
		prop_platform_switch.set_showing_style(RGB(200, 150, 255), PS_SOLID, 8);

		timer_platforms_switch.set_one_shot(true);
		timer_platforms_switch.set_wait_time(0.2f);
		timer_platforms_switch.pause();
		timer_platforms_switch.set_on_timeout([&]()
			{
				enabled_platforms = !enabled_platforms;
				for (auto platform : platform_list)
					platform->set_enabled(enabled_platforms);
			});
	}

	~SceneGameChoice()
	{

	}

	void on_enter() override
	{
		game_scene.on_enter();
		game_scene.stop_record();
		CharacterManager::instance()->get_player()->set_position(pos_player);

		// 初始化场景交互道具
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
		woodenman->set_position({ 550.0f, 620 - woodenman->get_img_size().y / 2 });

		platform_list.resize(5, nullptr);
		auto& platform_normal1 = platform_list[0];
		auto& platform_flash1 = platform_list[1];
		auto& platform_flash2 = platform_list[2];
		auto& platform_trans = platform_list[3];
		auto& platform_normal2 = platform_list[4];

		platform_normal1 = new Platform({ 100, 530 }, { 100, 20 });
		platform_normal2 = new Platform({ 260, 410 }, { 100, 20 });
		platform_flash1 = new Platform({ 400, 600 }, { 100, 40 });
		platform_flash2 = new Platform({ 550, 300 }, { 250, 150 });
		platform_flash1->set_mode(Platform::Mode::Flash);
		platform_flash2->set_mode(Platform::Mode::Flash);
		platform_trans = new Platform({ 70, 240 }, { 100, 40 });
		platform_trans->set_mode(Platform::Mode::Transmit);
		platform_trans->set_target_position({ 750, 300 });

		// 开启道具碰撞检测
		door_hornet->set_enabled(true);
		door_dragon_king->set_enabled(true);
		woodenman->set_enabled(true);
		for (auto platform : platform_list)
			platform->set_enabled(enabled_platforms);

		timer_platforms_switch.pause();
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

		timer_platforms_switch.on_update(delta);
	}
	void on_render() override
	{
		game_scene.render_background();	// 分层渲染，避免图层遮挡

		door_hornet->on_render();
		door_dragon_king->on_render();
		woodenman->on_render();
		for (auto platform : platform_list)
			platform->on_render();
		prop_platform_switch.on_render();

		CharacterManager::instance()->on_render();
		ParticleManager::instance()->on_render();
	}
	void on_exit() override
	{
		game_scene.clear_record();
		game_scene.on_exit();
		AudioManager::instance()->stop_audio_ex(_T("bgm_0"));

		// 销毁交互道具
		if (door_hornet) delete door_hornet;
		if (door_dragon_king) delete door_dragon_king;
		if (woodenman) delete woodenman;
		for (auto platform : platform_list)
			if (platform) delete platform;
	}
};
