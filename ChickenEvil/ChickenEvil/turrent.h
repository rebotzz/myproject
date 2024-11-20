#pragma once
#include "vector2.h"
#include "animation.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "resources_manager.h"
#include "bullet.h"
#include <ctime>
#include <memory>
#include <vector>

// ����
class Turrent
{
private:
	const float SHOT_CD = 0.15f;

private:
	Vector2 pos_self;									// ����λ��
	Vector2 pos_target;									// ���λ��
	Animation anim_barrel_fire;							// ǹ���������
	Animation anim_barrel_idle;							// ǹ�����ö�����ͼƬ��
	Animation anim_bottom;								// �����ײ�ʯͷ������ͼƬ��
	Animation anim_battery;								// �����ײ���ض�����ͼƬ��
	Animation anim_crosshair;							// ��׼ʮ�ֶ�����ͼƬ��
	Timer timer_shot;									// �ӵ�������
	bool is_shotting = false;							// ���״̬��
	std::vector<std::shared_ptr<Bullet>> bullet_list;	// �ӵ��б�

	bool shot_done = false;

	Animation* cur_anim = nullptr;
	Timer timer_shot_down;

public:
	Turrent(const Vector2& positon)
		:pos_self(positon)
	{
		anim_barrel_fire.set_loop(true);
		anim_barrel_fire.set_interval(0.05f);
		anim_barrel_fire.add_frame(ResourcesManager::instance()->find_atlas("barrel_fire"));
		anim_barrel_fire.set_position(pos_self + Vector2({100, 0}));
		anim_barrel_fire.set_rotate_center(25, 25);
		anim_barrel_fire.set_on_finished([&]()
			{
				if (!is_shotting)
				{
					timer_shot_down.restart();
				}
			});

		anim_barrel_idle.set_loop(false);
		anim_barrel_idle.set_interval(0.1f);
		anim_barrel_idle.add_frame(ResourcesManager::instance()->find_image("barrel_idle"), 1);
		anim_barrel_idle.set_position(pos_self + Vector2({ 100, 0 }));
		anim_barrel_idle.set_rotate_center(25, 25);

		anim_bottom.set_loop(false);
		anim_bottom.set_interval(0.1f);
		anim_bottom.add_frame(ResourcesManager::instance()->find_image("bottom"), 1);
		anim_bottom.set_position(pos_self);

		anim_battery.set_loop(false);
		anim_battery.set_interval(0.1f);
		anim_battery.add_frame(ResourcesManager::instance()->find_image("battery"), 1);
		anim_battery.set_position(pos_self);

		anim_crosshair.set_loop(false);
		anim_crosshair.set_interval(0.1f);
		anim_crosshair.add_frame(ResourcesManager::instance()->find_image("crosshair"), 1);
		anim_crosshair.set_position(pos_self);

		timer_shot.set_one_shot(false);
		timer_shot.set_wait_time(SHOT_CD);
		timer_shot.set_on_timeout([&]()
			{
				spawn_bullet();
			});

		timer_shot_down.set_one_shot(true);
		timer_shot_down.set_wait_time(0.04f);
		timer_shot_down.set_on_timeout([&]
			{
				cur_anim = &anim_barrel_idle;
			});

		cur_anim = &anim_barrel_idle;
	}

	void on_input(SDL_Event* event)
	{
		switch (event->type)
		{
		case SDL_MOUSEMOTION:
			pos_target.x = (float)event->button.x;
			pos_target.y = (float)event->button.y;
			break;
		case SDL_MOUSEBUTTONDOWN:								
			is_shotting = true;
			spawn_bullet();				// ������һ��Ҳ�������ӵ�.
			anim_barrel_fire.reset();
			cur_anim = &anim_barrel_fire;
			break;
		case SDL_MOUSEBUTTONUP:
			is_shotting = false;
			timer_shot.restart();
			break;
		}
	}

	void on_update(float delta)
	{
		// �����ӵ�,�л�/���¶���
		if (is_shotting)
		{
			timer_shot.on_update(delta);
			anim_barrel_fire.set_loop(true);
		}
		else
		{
			anim_barrel_fire.set_loop(false);
			timer_shot_down.on_update(delta);
		}
		cur_anim->on_update(delta);

		double shot_angle = (pos_target - pos_self).angle();
		anim_barrel_fire.set_angle(shot_angle);
		anim_barrel_idle.set_angle(shot_angle);
		anim_battery.set_angle(shot_angle);
		anim_crosshair.set_position(pos_target);

		// �����ӵ�
		for (auto& bullet : bullet_list)
		{
			bullet->on_update(delta);
		}

		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(), [](std::shared_ptr<Bullet> bullet)
			{
				return !bullet->is_valid();
			}), bullet_list.end());
	}

	void on_render(SDL_Renderer* renderer, const Camera& camera)
	{
		// �ӵ�
		for (auto& bullet : bullet_list)
		{
			bullet->on_render(renderer, camera);
		}

		// ����,ǹ�ܣ���׼
		anim_bottom.on_render(renderer, camera);
		anim_battery.on_render(renderer, camera);
		cur_anim->on_render(renderer, camera);
		anim_crosshair.on_render(renderer, camera);

		// ���ĵ�
		//SDL_Rect rect = { pos_self.x - 4, pos_self.y - 4, 8, 8 };
		//SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		//SDL_RenderFillRect(renderer, &rect);
	}

	void spawn_bullet()
	{
		// �����ӵ�	
		Vector2 pos_barrel_front = pos_self + (pos_target - pos_self).normalize() * 100.0;
		std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(pos_barrel_front, pos_target - pos_self);
		bullet_list.push_back(bullet);

		switch (rand() % 3 + 1)
		{
		case 1:
			Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio_chunk("fire_1"), 0);
			break;
		case 2:
			Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio_chunk("fire_2"), 0);
			break;
		case 3:
			Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio_chunk("fire_3"), 0);
			break;
		}
	}

};

