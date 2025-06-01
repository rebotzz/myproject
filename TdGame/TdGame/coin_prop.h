#pragma once

#include "vector2.h"
#include "timer.h"
#include "resources_manager.h"
#include "collision_manager.h"

class CoinMgr;

// ��ҵ���
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

		// λ�ø���
		if (is_jumping)
		{
			// ��ʼ��Ծ
			velocity.y += GRAVITY * delta;
			position += velocity * delta;
		}
		else
		{
			// �������¸���
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
		// ��΢����Χ���������ʰȡ
		return { position.x - 16 * 1.5f / 2, position.y - 16 * 1.5f / 2, 16 * 1.5f, 16 * 1.5f };
	}

private:
	bool is_valid = true;
	bool is_picked = false;							// �Ƿ�ʰȡ
	bool is_jumping = true;							// �Ƿ��ǳ�ʼ"�����"����Ծ״̬
	CollisionBox* coin_box = nullptr;				// ʰȡ��ײ����
	Timer timer_disappear;							// ʧЧ��ʱ��
	Timer timer_jump;								// ��ʼ"�����"����Ծ״̬��ʱ��
	float acculate_delta = 0;						// �������¸���sin()������
	Vector2 velocity;								// �ٶ�
	Vector2 position;								// λ��
	int hover_y = 0;								// ���¸���������λ��
	const int GRAVITY = 3.5 * TILE_SIZE;			// �������ٶ�
	const int INIT_SPEED = 2.4 * TILE_SIZE;			// ��ʼ"�����"�ĳ�ʼ�ٶ�
};
