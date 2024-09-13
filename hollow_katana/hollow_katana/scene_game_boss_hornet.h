#pragma once
#include <memory>
#include "scene.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "particle_manager.h"
#include "reverse_time_manager.h"
#include "audio_manager.h"
#include "bullet_time_manager.h"
#include "enemy_hornet.h"


class SceneGameBossHornet : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	Vector2 pos_hornet = { 1050, 620 };
	IMAGE* background = nullptr;
	Rect rect_dst;

public:
	SceneGameBossHornet()
	{
		background = ResourcesManager::instance()->find_image("background_hollow");
		rect_dst.x = (getwidth() - rect_dst.w) / 2;
		rect_dst.y = (getheight() - rect_dst.h) / 2;
		rect_dst.w = background->getwidth(), rect_dst.h = background->getheight();
	}

	void on_enter() override
	{
		CharacterManager::instance()->set_enable(true);

		// 改为智能指针,避免不能正确调用析构
		std::shared_ptr<EnemyHornet> hornet(new EnemyHornet);
		CharacterManager::instance()->create_enemy("hornet", hornet);
		CharacterManager::instance()->get_enemy("hornet")->set_position(pos_hornet);

		auto* player = CharacterManager::instance()->get_player();
		player->reset();
		player->switch_state("idle");
		player->set_position(pos_player);

		ReverseTimeManager::instance()->set_enable(true);
		AudioManager::instance()->play_audio_ex(_T("bgm_1"), true);
	}

	void on_input(const ExMessage& msg) override
	{
		CharacterManager::instance()->on_input(msg);
	}

	void on_update(float delta) override
	{
		CharacterManager::instance()->on_update(delta);
		ParticleManager::instance()->on_update(delta);

		if (CharacterManager::instance()->get_player()->get_hp() <= 0)
			ReverseTimeManager::instance()->set_record(false);
		else
			ReverseTimeManager::instance()->set_record(true);
		ReverseTimeManager::instance()->on_update(delta);

		if (BulletTimeManager::instance()->is_enable())
			AudioManager::instance()->pause_audio_ex(_T("bgm_1"));
		else 
			AudioManager::instance()->resume_audio_ex(_T("bgm_1"));
	}
	void on_render() override
	{
		render_background();
		CharacterManager::instance()->on_render();
		ParticleManager::instance()->on_render();
	}

	void on_exit() override
	{
		CharacterManager::instance()->destroy_enemy("hornet");
		CharacterManager::instance()->set_enable(false);
		ReverseTimeManager::instance()->set_enable(false);

		AudioManager::instance()->stop_audio_ex(_T("bgm_1"));
	}

	void render_background()
	{
		setbkcolor(RGB(0, 0, 0));
		putimage_alpha_ex(background, &rect_dst);
	}
};
