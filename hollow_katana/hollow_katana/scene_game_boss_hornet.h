#pragma once
#include "scene.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "particle_manager.h"
#include "enemy_hornet.h"

class SceneGameBossHornet : public Scene
{
public:

	void on_enter()
	{
		CharacterManager::instance()->create_enemy("hornet", new EnemyHornet);
		CharacterManager::instance()->set_enable(true);
	}

	void on_input(const ExMessage& msg)  
	{
		CharacterManager::instance()->on_input(msg);
	}

	void on_update(int delta)  
	{
		CharacterManager::instance()->on_update(delta);
		ParticleManager::instance()->on_update(delta);
	}
	void on_render()  
	{
		render_background();
		CharacterManager::instance()->on_render();
		ParticleManager::instance()->on_render();
	}

	void on_exit() 
	{
		CharacterManager::instance()->destroy_enemy("hornet");
		CharacterManager::instance()->set_enable(false);
	}

	void render_background()
	{
		static IMAGE* image = ResourcesManager::instance()->find_image("background");
		static Rect rect_dst;
		rect_dst.x = (getwidth() - rect_dst.w) / 2;
		rect_dst.y = (getheight() - rect_dst.h) / 2;
		rect_dst.w = image->getwidth(), rect_dst.h = image->getheight();

		setbkcolor(RGB(0, 0, 0));
		putimage_alpha_ex(image, &rect_dst);
	}
};
