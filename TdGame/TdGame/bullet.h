#pragma once

#include "vector2.h"
#include "animation.h"
#include "collision_manager.h"
#include "enemy.h"

#include "SDL.h"

class BulletMgr;

class Bullet
{
	friend class BulletMgr;
public:
	virtual void on_update(float delta)
	{
		if (!is_valid) return;

		// Î»ÖÃ
		position += velocity * delta;
		anim.set_position(position);
		hit_box->set_position(position);

		// ¶¯»­
		anim.on_update(delta);
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		if (!is_valid) return;
		anim.on_render(renderer);
	}

	int get_damage() const
	{
		return damage;
	}

	void set_damage(int val)
	{
		this->damage = val;
	}

	void make_invalid()
	{
		is_valid = false;
		hit_box->set_valid(false);
	}

	bool can_remove() const
	{
		return !is_valid;
	}

	void set_slow_scale(float val)
	{
		slow_debuff = true;
		slow_scale = val;
	}

	void set_velocity(const Vector2& velocity)
	{
		this->velocity = velocity;
	}

	void set_position(const Vector2& position)
	{
		this->position = position;
		hit_box->set_position(position);
	}

	const Vector2& get_position() const
	{
		return position;
	}

protected:
	Bullet()
	{
		// Åö×²ºÐ×Ó
		hit_box = CollisionMgr::instance()->create_collision_box();
		hit_box->set_dst_layer(CollisionLayer::Enemy);
		hit_box->set_size({ 3, 3 });
		hit_box->set_on_collide([this]()
			{
				Enemy* enemy = reinterpret_cast<Enemy*>(hit_box->get_on_collide_box()->get_user_data());
				if (!enemy || enemy->can_remove()) return;
				enemy->decrease_hp(damage);
				if (enemy->can_remove()) return;
				if (slow_debuff)
				{
					enemy->slow(slow_scale);
				}
				make_invalid();
				play_hit_sound();
			});
	}
	Bullet(const Bullet&) = delete;
	const Bullet& operator=(const Bullet&) = delete;
	virtual ~Bullet()
	{
		CollisionMgr::instance()->destroy_collision_box(hit_box);
	}

	virtual void play_hit_sound()
	{}

protected:
	Vector2 velocity;
	Vector2 position;
	int damage = 10;
	bool is_valid = true;
	bool slow_debuff = false;
	float slow_scale = 0.5f;
	Animation anim;
	CollisionBox* hit_box = nullptr;
};

