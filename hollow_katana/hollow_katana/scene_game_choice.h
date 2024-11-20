#pragma once
#include <memory>
#include <vector>
#include "scene.h"
#include "scene_game.h"
#include "resources_manager.h"
#include "door.h"
#include "woodenman.h"
#include "platform.h"




// v1.3����:������,ľͷ��,ƽ̨������

// todo: 
// 1.���ӵ���,��ֹ��ɫ�ɳ���Ļ����
// 2.��������ʱ,������������,��ײ���巽��,ê��λ��,��������Ծ����(��ʱ����,�����ڱ����Ϸ��ʵ��)

// TODO:������ö�̬���أ��õ�ʱ���ڴ�����һ����ȫ�������ײ����ܶ���ײ�� | ���߳������زĽ�����أ��˳�����
class SceneGameChoice : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	GameScene game_scene;

	// �������ܵ���
	Door* door_hornet = nullptr;							
	Door* door_dragon_king = nullptr;
	Woodenman* woodenman = nullptr;
	std::vector<Platform*> platform_list;
	bool enabled_platforms = false;
	InteractProp prop_platform_switch;
	Timer timer_switch_platforms;

public:
	SceneGameChoice()
		:game_scene(ResourcesManager::instance()->find_image("background_katana"), _T("bgm_0"))
	{
		// û��ս����ֹͣ�����¼
		game_scene.stop_record();

		prop_platform_switch.set_describe(_T("��Ծƽ̨����"));
		prop_platform_switch.set_on_interact([&]()
			{
				timer_switch_platforms.resume();
				timer_switch_platforms.restart();
			});
		prop_platform_switch.set_size({ 20, 70 });
		prop_platform_switch.set_position({ 720, 580 });
		prop_platform_switch.enable_showing_box(true);
		prop_platform_switch.set_showing_style(RGB(200, 150, 255), PS_SOLID, 8);

		timer_switch_platforms.set_one_shot(true);
		timer_switch_platforms.set_wait_time(0.2f);
		timer_switch_platforms.pause();
		timer_switch_platforms.set_on_timeout([&]()
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

		// ��ʼ��������������
		door_hornet = new Door("game_scene_boss_hornet");
		door_hornet->set_describe(_T("boss��Ʒ�"));
		door_hornet->set_size({ 60, 120 });
		door_hornet->set_position({ 870, 560 });
		door_hornet->set_color(RGB(255, 50, 50));

		door_dragon_king = new Door("game_scene_boss_dragon_king");
		door_dragon_king->set_describe(_T("boss����"));
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

		// ����������ײ���
		door_hornet->set_enabled(true);
		door_dragon_king->set_enabled(true);
		woodenman->set_enabled(true);
		for (auto platform : platform_list)
			platform->set_enabled(enabled_platforms);

		timer_switch_platforms.pause();
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

		timer_switch_platforms.on_update(delta);
	}
	void on_render() override
	{
		//game_scene.on_render();
		game_scene.render_background();	// �ֲ���Ⱦ������ͼ���ڵ�

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

		// ���ٽ�������
		if (door_hornet) delete door_hornet;
		if (door_dragon_king) delete door_dragon_king;
		if (woodenman) delete woodenman;
		for (auto platform : platform_list)
			if (platform) delete platform;

		//// �رյ�����ײ���
		//door_hornet->set_enabled(false);
		//door_dragon_king->set_enabled(false);
		//woodenman->set_enabled(false);
		//for (auto platform : platform_list)
		//	platform->set_enabled(false);
	}
};
