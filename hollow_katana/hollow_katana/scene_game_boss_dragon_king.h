#pragma once
#include <memory>
#include "scene.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "particle_manager.h"
#include "reverse_time_manager.h"
#include "enemy_dragon_king.h"


class SceneGameBossDragonKing : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	Vector2 pos_enemy = { 1050, 620 };

public:

	void on_enter() override
	{
		cout << "进入: SceneGameBossDragonKing" << endl;

		CharacterManager::instance()->set_enable(true);

		// 改为智能指针,避免不能正确调用析构
		std::shared_ptr<EnemyDragonKing> dragon_king(new EnemyDragonKing);
		CharacterManager::instance()->create_enemy("dragon_king", dragon_king);
		CharacterManager::instance()->get_enemy("dragon_king")->set_position(pos_enemy);

		auto* player = CharacterManager::instance()->get_player();
		player->reset();
		player->switch_state("idle");
		player->set_position(pos_player);

		ReverseTimeManager::instance()->set_enable(true);
	}

	void on_input(const ExMessage& msg) override
	{
		CharacterManager::instance()->on_input(msg);
	}

	void on_update(float delta) override
	{
		CharacterManager::instance()->on_update(delta);
		ParticleManager::instance()->on_update(delta);

		ReverseTimeManager::instance()->on_update(delta);
	}
	void on_render() override
	{
		render_background();
		CharacterManager::instance()->on_render();
		ParticleManager::instance()->on_render();
	}

	void on_exit() override
	{
		CharacterManager::instance()->destroy_enemy("dragon_king");
		CharacterManager::instance()->set_enable(false);

		ReverseTimeManager::instance()->set_enable(false);
	}

	void render_background()
	{
		static IMAGE* image = ResourcesManager::instance()->find_image("background_katana");
		static Rect rect_dst;
		rect_dst.x = (getwidth() - rect_dst.w) / 2;
		rect_dst.y = (getheight() - rect_dst.h) / 2;
		rect_dst.w = image->getwidth(), rect_dst.h = image->getheight();

		setbkcolor(RGB(0, 0, 0));
		putimage_alpha_ex(image, &rect_dst);
	}
};
