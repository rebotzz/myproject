#pragma once
#include "SDL.h"
#include "vector2.h"
#include "timer.h"

// v0.1:��������ж�λ����
// v0.2:�����ͬʱ��Ϊ��ͼ�ķ����ߺͻ��ƽ����������Ϸ�������ţ�����ͶӰ

extern float win_scale;

class Camera
{
private:
	Vector2 pos_cur;					// ��ǰ�����λ��
	Vector2 pos_base;					// ��׼�����λ��
	Vector2 size;						// ���ڴ�С???
	bool is_shake = false;				// �Ƿ񶶶�
	int shake_strength = 0;				// ����ǿ��
	Timer timer_shake;					// ������ʱ��
	SDL_Renderer* renderer = nullptr;	// ��Ⱦ��

public:
	Camera(SDL_Renderer* renderer_)
		:renderer(renderer_)
	{
		timer_shake.set_one_shot(false);
		timer_shake.set_on_timeout([&]()
			{
				is_shake = false;
				pos_cur = pos_base;
			});
	}
	Camera()
	{
		timer_shake.set_one_shot(false);
		timer_shake.set_on_timeout([&]()
			{
				is_shake = false;
				pos_cur = pos_base;
			});
	}
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
	}

	void on_update(float delta)
	{
		if (is_shake)
		{
			timer_shake.on_update(delta);
			pos_cur.x = (-50 + rand() % 100) / 50.0f * shake_strength + pos_base.x;
			pos_cur.y = (-50 + rand() % 100) / 50.0f * shake_strength + pos_base.y;
		}
	}

	void render_texture(SDL_Texture* texture, const SDL_Rect* rect_src, const SDL_FRect* rect_dst,
		float angle, const SDL_FPoint* center) const
	{
		SDL_FRect rect_dst_win = *rect_dst;
		rect_dst_win.x -= pos_cur.x;
		rect_dst_win.y -= pos_cur.y;

		// ���Ŵ��ڴ�С
		rect_dst_win.x *= win_scale, rect_dst_win.y *= win_scale;
		rect_dst_win.w *= win_scale, rect_dst_win.h *= win_scale;
		SDL_FPoint center_win = { center ? center->x * win_scale : 0, center ? center->y * win_scale : 0};

		SDL_RenderCopyExF(renderer, texture, rect_src, &rect_dst_win, angle, center ? &center_win : nullptr, SDL_FLIP_NONE);
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