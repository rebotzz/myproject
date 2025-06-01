#pragma once

#include "timer.h"
#include "vector2.h"

#include "SDL.h"	

// todo: ��Ϸ��������ת��δ��ɣ��������ʱ��ǳ

class Camera
{
public:
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


	void on_render(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, 
		const double angle = 0.0, const SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		SDL_Rect dstrect_win = *dstrect;
		dstrect_win.x -= pos_cur.x;
		dstrect_win.y -= pos_cur.y;

		SDL_RenderCopyEx(renderer, texture, srcrect, &dstrect_win, angle, center, flip);
	}

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

	SDL_Renderer* get_renderer() const
	{
		return renderer;
	}

	void set_renderer(SDL_Renderer* renderer)
	{
		this->renderer = renderer;
	}

private:
	Vector2 pos_cur;					// ��ǰ�����λ��
	Vector2 pos_base;					// ��׼�����λ��
	Vector2 size;						// ���ڴ�С
	bool is_shake = false;				// �Ƿ񶶶�
	int shake_strength = 0;				// ����ǿ��
	Timer timer_shake;					// ������ʱ��
	SDL_Renderer* renderer = nullptr;	// ��Ⱦ��
};

