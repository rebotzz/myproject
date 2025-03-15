#pragma once
#include "SDL.h"
#include "vector2.h"
#include "timer.h"

// v0.1:摄像机仅有定位功能
// v0.2:摄像机同时作为绘图的发出者和绘制结果，便于游戏画面缩放，镜面投影

extern float win_scale;

class Camera
{
private:
	Vector2 pos_cur;					// 当前摄像机位置
	Vector2 pos_base;					// 基准摄像机位置
	Vector2 size;						// 窗口大小???
	bool is_shake = false;				// 是否抖动
	int shake_strength = 0;				// 抖动强度
	Timer timer_shake;					// 抖动计时器
	SDL_Renderer* renderer = nullptr;	// 渲染器

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

	// 窗口坐标 = 世界坐标 - 摄像机坐标
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

		// 缩放窗口大小
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