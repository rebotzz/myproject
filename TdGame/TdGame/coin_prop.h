#pragma once

#include "vector2.h"
#include "timer.h"
#include "resources_manager.h"
#include "collision_manager.h"

class CoinMgr;

// 金币道具
class CoinProp
{
	friend class CoinMgr;
public:
	CoinProp(const Vector2& postion)
		: position(postion)
	{
		coin_box = CollisionMgr::instance()->create_collision_box();
		coin_box->set_src_layer(CollisionLayer::Prop);
		coin_box->set_size({ 16,16 });
		coin_box->set_on_collide([this]()
			{
				pick_up();
			});

		timer_disappear.set_one_shot(true);
		timer_disappear.set_wait_time(30);
		timer_disappear.set_on_timeout([this]()
			{
				is_valid = false;
			});
		timer_jump.set_one_shot(true);
		timer_jump.set_wait_time(1.4);
		timer_jump.set_on_timeout([&]()
			{
				is_jumping = false;
				hover_y = position.y;
			});

		velocity = Vector2(rand() % 2 ? 1 : -1, -1.5).normalize() * INIT_SPEED;
	}
	~CoinProp()
	{
		CollisionMgr::instance()->destroy_collision_box(coin_box);
	}

	void on_update(float delta)
	{
		timer_disappear.on_update(delta);
		timer_jump.on_update(delta);

		// 位置更新
		if (is_jumping)
		{
			// 初始跳跃
			velocity.y += GRAVITY * delta;
			position += velocity * delta;
		}
		else
		{
			// 后续上下浮动
			acculate_delta += delta;
			position.y = hover_y + sin(acculate_delta * 1.3) * 10;
		}
		coin_box->set_position(position);
	}

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Texture* tex_coin = ResMgr::instance()->find_texture(ResID::Tex_Coin);
		SDL_Rect rect = { position.x - 16 / 2, position.y - 16 / 2, 16, 16 };
		SDL_RenderCopy(renderer, tex_coin, nullptr, &rect);
	}

	bool can_remove() const
	{
		return !is_valid;
	}

	bool is_picked_up() const
	{
		return is_picked;
	}

	void pick_up()
	{
		is_picked = true;
		is_valid = false;
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_Coin), 0);
	}

	SDL_FRect get_rect() const
	{
		// 略微扩大范围，方便鼠标拾取
		return { position.x - 16 * 1.5f / 2, position.y - 16 * 1.5f / 2, 16 * 1.5f, 16 * 1.5f };
	}

private:
	bool is_valid = true;
	bool is_picked = false;							// 是否被拾取
	bool is_jumping = true;							// 是否是初始"暴金币"的跳跃状态
	CollisionBox* coin_box = nullptr;				// 拾取碰撞盒子
	Timer timer_disappear;							// 失效定时器
	Timer timer_jump;								// 初始"暴金币"的跳跃状态定时器
	float acculate_delta = 0;						// 用于上下浮动sin()的输入
	Vector2 velocity;								// 速度
	Vector2 position;								// 位置
	int hover_y = 0;								// 上下浮动的中心位置
	const int GRAVITY = 3.5 * TILE_SIZE;			// 重力加速度
	const int INIT_SPEED = 2.4 * TILE_SIZE;			// 初始"暴金币"的初始速度
};
