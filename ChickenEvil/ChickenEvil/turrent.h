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

// 炮塔
class Turrent
{
private:
	const float SHOT_CD = 0.1f;


private:
	Vector2 pos_self;
	Vector2 pos_target;
	Animation anim;
	bool is_shotting = false;
	Timer timer_shot;
	double shot_angle = 0.0f;

	SDL_Surface* suf_img_battery = nullptr;		// 炮塔底部
	SDL_Rect rect_battery = { 0 };
	SDL_Surface* suf_img_crosshair = nullptr;	// 瞄准
	SDL_Rect rect_crosshair = { 0 };

	std::vector<std::shared_ptr<Bullet>> bullet_list;

public:
	Turrent(const Vector2& positon)
		:pos_self(positon)
	{
		anim.set_loop(true);
		anim.set_interval(0.1f);
		anim.add_frame(ResourcesManager::instance()->find_atlas("barrel_fire"));
		anim.set_position(pos_self + Vector2({80, 100}));
		anim.set_rotate_center(25, 25);

		timer_shot.set_one_shot(false);
		timer_shot.set_wait_time(SHOT_CD);
		timer_shot.set_on_timeout([&]()
			{
				// 生成子弹
				std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(pos_self, pos_target);
				bullet_list.push_back(bullet);
			});

		suf_img_battery = ResourcesManager::instance()->find_image("battery");
		rect_battery.w = suf_img_battery->w;
		rect_battery.h = suf_img_battery->h;
		rect_battery.x = pos_self.x - 50;
		rect_battery.y = pos_self.y;


		// todo: 打印调整图形相对位置
		//SDL_Log("")
	}

	void on_input(SDL_Event* event)
	{
		switch (event->type)
		{
		case SDL_MOUSEMOTION:
			pos_target.x = (float)event->button.x;
			pos_target.y = (float)event->button.y;
			break;
		case SDL_MOUSEBUTTONDOWN:								// todo: 优化，单独按一次也会生成子弹
			is_shotting = true;
			break;
		case SDL_MOUSEBUTTONUP:
			is_shotting = false;
			break;
		}
	}

	void on_update(float delta)
	{
		// 发射子弹,更新动画
		if (is_shotting)
		{
			timer_shot.on_update(delta);
			anim.set_loop(true);
		}
		else
		{
			anim.set_loop(false);
		}
		anim.on_update(delta);
		shot_angle = (pos_target - pos_self).angle();
		anim.set_angle(shot_angle);

		// 更新子弹
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

		SDL_Texture* tex_img_battery = SDL_CreateTextureFromSurface(renderer, suf_img_battery);

		SDL_Rect rect =
		{
			rect_battery.x - (int)camera.get_position().x,
			rect_battery.y - (int)camera.get_position().y,
			rect_battery.w, rect_battery.h
		};
		SDL_RenderCopyEx(renderer, tex_img_battery, nullptr, &rect, shot_angle, nullptr, SDL_FLIP_NONE);
		SDL_DestroyTexture(tex_img_battery);

		anim.on_render(renderer, camera);

		// 子弹
		for (auto& bullet : bullet_list)
		{
			bullet->on_render(renderer, camera);
		}

	}

};

