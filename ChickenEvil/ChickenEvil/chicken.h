#pragma once
#include "vector2.h"
#include "animation.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "resources_manager.h"
#include <ctime>
#include <memory>

extern const int WINDOW_W;
extern const int WINDOW_H;

class Chicken
{
protected:
	int hp = 1;
	Vector2 position;
	float speed_y = 30.0f;
	Animation anim;
	CollisionBox* hurt_box = nullptr;

public:
	Chicken()
	{
		// 随机位置刷新
		position.x = (rand() % (WINDOW_W - 32)) + 32 / 2;
		position.y = -55;

		//position.x = 600;
		//position.y = 350;

		anim.set_interval(0.3f);
		anim.set_loop(true);
		anim.set_position(position);

		hurt_box = CollisionManager::instance()->create_collision_box();
		hurt_box->set_enabled(true);
		hurt_box->set_layer_src(CollisionLayer::Enemy);
		hurt_box->set_layer_dst(CollisionLayer::None);
		hurt_box->set_position(position);
		hurt_box->set_size({ 30, 40 });
		hurt_box->set_on_collision([&]()
			{
				hp = 0;
				//SDL_Log("chicken died, x:%f, y:%f\n", position.x, position.y);
			});

		//SDL_Log("Create a Chicken, x: %f, y:%f\n", position.x, position.y);

	}
	~Chicken()
	{
		CollisionManager::instance()->destroy_collision_box(hurt_box);

		//SDL_Log("Delete Chicken, x: %f, y:%f\n", position.x, position.y);
	}

	void on_update(float delta)
	{
		if (!is_alive())
			return;

		position.y += speed_y * delta;
		hurt_box->set_position(position);
		anim.set_position(position);
		anim.on_update(delta);

		// 超过屏幕底部消失，给玩家扣血 ->放到检测位置
	}

	void on_render(SDL_Renderer* renderer, const Camera& camera)
	{
		if (!is_alive())
			return;

		anim.on_render(renderer, camera);
	}

	bool is_alive() const
	{
		return hp > 0;
	}

	void dead()
	{
		hp = 0;
	}

	const Vector2& get_position() const
	{
		return position;
	}

};


class ChickenFast : public Chicken
{
public:
	ChickenFast()
		:Chicken()
	{
		anim.add_frame(ResourcesManager::instance()->find_atlas("chicken_fast"));
		speed_y *= 1.2f;
	}
};

class ChickenMedium : public Chicken
{
public:
	ChickenMedium()
		:Chicken()
	{
		anim.add_frame(ResourcesManager::instance()->find_atlas("chicken_medium"));
	}
};

class ChickenSlow : public Chicken
{
public:
	ChickenSlow()
		:Chicken()
	{
		anim.add_frame(ResourcesManager::instance()->find_atlas("chicken_slow"));
		speed_y *= 0.7f;
	}
};


