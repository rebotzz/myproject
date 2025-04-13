#pragma once
#include "SDL.h"
#include "timer.h"
#include "vector2.h"

// 动画类
class Animation
{
private:
	// 动画帧
	struct Frame
	{
		SDL_Rect rect_src;
		SDL_Texture* tex_img = nullptr;

		Frame() = default;
		Frame(SDL_Texture* img, SDL_Rect rect)
			:tex_img(img), rect_src(rect) {
		}
		~Frame() = default;
	};

private:
	Timer timer;										// 动画播放发计时器		
	Vector2 position;									// 动画位置(中心锚点)
	size_t idx_frame = 0;								// 帧索引,当前正在播放的动画帧
	bool is_loop = false;								// 是否循环播放					
	std::vector<Frame> frame_list;						// 动画帧序列
	std::function<void()> on_finished;					// 动画结束处理	
	float angle = 0.0f;									// 动画旋转角度
	SDL_FPoint rotate_center = { 0 };					// 旋转中心

public:
	Animation()
	{
		timer.set_one_shot(false);
		timer.set_on_timeout(
			[&]()
			{
				idx_frame++;
				if (idx_frame >= frame_list.size())
				{
					idx_frame = is_loop ? 0 : frame_list.size() - 1;
					if (!is_loop && on_finished)
						on_finished();
				}
			}
		);
	}
	~Animation() = default;

	void reset()
	{
		timer.restart();
		idx_frame = 0;
	}

	void set_on_finished(const std::function<void()>& callback)
	{
		on_finished = callback;
	}

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	void set_interval(float val)
	{
		timer.set_wait_time(val);
	}

	void set_loop(bool flag)
	{
		is_loop = flag;
	}

	void set_angle(double val)
	{
		angle = val;
	}

	void set_rotate_center(float x, float y)
	{
		rotate_center.x = x;
		rotate_center.y = y;
	}

	void on_update(float delta)
	{
		timer.on_update(delta);
	}

	void on_render(SDL_Renderer* renderer) const
	{
		const Frame& frame = frame_list[idx_frame];

		SDL_FRect rect_dst;
		rect_dst.w = static_cast<float>(frame.rect_src.w);
		rect_dst.h = static_cast<float>(frame.rect_src.h);
		rect_dst.x = static_cast<float>(position.x - frame.rect_src.w / 2);		// 中心锚点模式
		rect_dst.y = static_cast<float>(position.y - frame.rect_src.h / 2);

		SDL_RenderCopyExF(renderer, frame.tex_img, &frame.rect_src, &rect_dst, angle, &rotate_center, SDL_FLIP_NONE);
	}

	void add_frame(SDL_Texture* tex_img, int num_w = 1)
	{
		int height = 0, width = 0;
		SDL_QueryTexture(tex_img, nullptr, nullptr, &width, &height);
		int width_frame = width / num_w;

		for (int i = 0; i < num_w; ++i)
		{
			SDL_Rect rect_src;
			rect_src.x = width_frame * i, rect_src.y = 0;
			rect_src.w = width_frame, rect_src.h = height;

			frame_list.emplace_back(tex_img, rect_src);
		}
	}

};