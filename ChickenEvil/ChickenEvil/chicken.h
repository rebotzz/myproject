#pragma once
#include "animation.h"
#include "collision_manager.h"
#include "resources_manager.h"
#include "particle_manager.h"
#include <ctime>
#include <memory>

extern const int WINDOW_W;
extern const int WINDOW_H;

// 生化鸡
class Chicken
{
protected:
	Vector2 position;							// 位置
	float speed_y = 35.0f;						// 移动速度
	Animation anim_chicken;						// 动画
	CollisionBox* hurt_box = nullptr;			// 受击碰撞箱
	bool is_alive = true;						// 存活状态

public:
	Chicken()
	{
		// 随机位置刷新
		position.x = (float)((rand() % (WINDOW_W - 32)) + 32 / 2);
		position.y = -55.0f;

		anim_chicken.set_interval(0.3f);
		anim_chicken.set_loop(true);
		anim_chicken.set_position(position);

		hurt_box = CollisionManager::instance()->create_collision_box();
		hurt_box->set_enabled(true);
		hurt_box->set_layer_src(CollisionLayer::Enemy);
		hurt_box->set_layer_dst(CollisionLayer::None);
		hurt_box->set_position(position);
		hurt_box->set_size({ 30, 40 });
		hurt_box->set_on_collision([&]()
			{
				if (is_alive)		// 爆炸状态只存在一瞬间，用于生成粒子;优化，只用是否存活表示
				{
					is_alive = false;
					// 死亡，放烟花(创建粒子特效)
					std::shared_ptr<Particle> particle = std::shared_ptr<Particle>(new ParticleEffExplode(position));
					ParticleManager::instance()->register_particle(particle);
					Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio_chunk("explosion"), 0);
				}
			});
	}
	~Chicken()
	{
		CollisionManager::instance()->destroy_collision_box(hurt_box);
	}

	void on_update(float delta)
	{
		if (!check_alive())
			return;

		position.y += speed_y * delta;
		hurt_box->set_position(position);
		anim_chicken.set_position(position);
		anim_chicken.on_update(delta);
	}

	void on_render(const Camera& camera) const
	{
		if (!check_alive())
			return;

		anim_chicken.on_render(camera);
	}

	bool check_alive() const
	{
		return is_alive;
	}

	bool can_remove() const
	{
		return !is_alive || (position.y > (float)WINDOW_H + 50.0f);
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
		anim_chicken.add_frame(ResourcesManager::instance()->find_atlas("chicken_fast"));
		speed_y *= 1.5f;
	}
};

class ChickenMedium : public Chicken
{
public:
	ChickenMedium()
		:Chicken()
	{
		anim_chicken.add_frame(ResourcesManager::instance()->find_atlas("chicken_medium"));
	}
};

class ChickenSlow : public Chicken
{
public:
	ChickenSlow()
		:Chicken()
	{
		anim_chicken.add_frame(ResourcesManager::instance()->find_atlas("chicken_slow"));
		speed_y *= 0.6f;
	}
};


