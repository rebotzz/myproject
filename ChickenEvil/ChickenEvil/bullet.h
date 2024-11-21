#pragma once
#include "collision_manager.h"
#include "resources_manager.h"
#include "animation.h"

extern const int WINDOW_W;
extern const int WINDOW_H;

// 子弹类
class Bullet
{
private:
	const float SPEED = 600.0f;

private:
	Animation anim;
	CollisionBox* hit_box = nullptr;
	Vector2 pos;
	Vector2 velocity;
	bool valid = true;

public:
	Bullet(const Vector2& pos_src, const Vector2& direction)
	{
		pos = pos_src;
		velocity = direction.normalize() * SPEED;

		anim.add_frame(ResourcesManager::instance()->find_image("bullet"));
		anim.set_loop(false);
		anim.set_interval(0.1f);
		anim.set_position(pos);
		anim.set_angle(velocity.angle());

		hit_box = CollisionManager::instance()->create_collision_box();
		hit_box->set_enabled(true);
		hit_box->set_layer_dst(CollisionLayer::Enemy);
		hit_box->set_position(pos);
		hit_box->set_size({ 4, 4 });
		hit_box->set_on_collision([&]()
			{
				valid = false;
			});
	}

	~Bullet()
	{
		CollisionManager::instance()->destroy_collision_box(hit_box);
	}

	void on_update(float delta)
	{
		pos += velocity * delta;
		anim.set_position(pos);
		hit_box->set_position(pos);

		// 如果超过屏幕边界
		if (pos.x < -5.0f || pos.x > WINDOW_W + 5.0f
			|| pos.y < -5.0f || pos.y > WINDOW_H + 5.0f)
		{
			valid = false;
		}
	}

	void on_render(SDL_Renderer* renderer, const Camera& camera)
	{
		anim.on_render(renderer, camera);
	}

	bool is_valid() const
	{
		return valid;
	}
};
