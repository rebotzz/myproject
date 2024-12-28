#pragma once
#include "SDL.h"
#include "vector2.h"
#include "timer.h"

// v0.1:��������ж�λ����
// v0.2:�����ͬʱ��Ϊ��ͼ�ķ����ߺͻ��ƽ����������Ϸ�������ţ�����ͶӰ

class Camera
{
private:
	Vector2 pos_cur;					// ��ǰ�����λ��
	Vector2 pos_base;					// ��׼�����λ��
	Vector2 size;						// ���ڴ�С???
	volatile bool is_shake = false;		// �Ƿ񶶶�
	int shake_strength = 0;				// ����ǿ��
	Timer timer_shake;					// ������ʱ��
	SDL_Renderer* renderer = nullptr;	// ��Ⱦ��

public:
	Camera(SDL_Renderer* renderer_)
		:renderer(renderer_)
	{
		timer_shake.set_one_shot(true);
		timer_shake.set_on_timeout([&]()
			{
				is_shake = false;
				pos_cur = pos_base;

				SDL_Log("timer_shake over!!!!!!!!!!!!.\n");
			});
	}
	Camera() = default;
	~Camera() = default;

	void set_size(const Vector2& val)
	{
		size = val;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void set_position(const Vector2& val)
	{
		pos_base = pos_cur = val;
	}

	const Vector2& get_position() const
	{
		return pos_cur;
	}

	// �������� = �������� - ���������
	void look_at(const Vector2& target)
	{
		pos_cur = target - size / 2.0f;
	}

	void shake(float strenth, float duration)
	{
		is_shake = true;
		shake_strength = strenth;

		timer_shake.set_wait_time(duration);
		timer_shake.restart();

		SDL_Log("camera shake start---------- duration: [%f].\n", duration);
	}

	void on_update(float delta)
	{
		if (is_shake)
		{
			timer_shake.on_update(delta);
			pos_cur.x = (-50 + rand() % 100) / 50.0f * shake_strength + pos_base.x;
			pos_cur.y = (-50 + rand() % 100) / 50.0f * shake_strength + pos_base.y;
		}

		//SDL_Log("camera shake: [%d], delta: %f\n", is_shake, delta);
		SDL_Log("camera pass time: [%f]\n", timer_shake.get_pass_time());
	}

	void render_texture(SDL_Texture* texture, const SDL_Rect* rect_src, const SDL_FRect* rect_dst,
		float angle, const SDL_FPoint* center) const
	{
		SDL_FRect rect_dst_win = *rect_dst;
		rect_dst_win.x -= pos_cur.x;
		rect_dst_win.y -= pos_cur.y;

		SDL_RenderCopyExF(renderer, texture, rect_src, &rect_dst_win, angle, center, SDL_FLIP_NONE);
	}

	SDL_Renderer* get_renderer() const
	{
		return renderer;
	}

	void set_renderer(SDL_Renderer* renderer_)
	{
		renderer = renderer_;
	}
};