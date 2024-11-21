#pragma once
#include "atlas.h"
#include "vector2.h"
#include "timer.h"
#include "camera.h"

// 用SDL_Surface还是SDL_Texture
//
// v0.1:动画首次渲染时SDL_Surface转SDL_Texture, 之后照常，动画析构时释放SDL_Texture
// v0.2:改用SDL_Texture，频繁创建/销毁太耗性能,通过SDL_QueryTexture查看纹理信息

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
			:tex_img(img), rect_src(rect) {}
		~Frame() = default;
	};

private:
	Timer timer;										// 动画播放发计时器		
	Vector2 position;									// 动画位置(中心锚点)
	size_t idx_frame = 0;								// 帧索引,当前正在播放的动画帧
	bool is_loop = false;								// 是否循环播放					
	std::vector<Frame> frame_list;						// 动画帧序列
	std::function<void()> on_finished;					// 动画结束处理	
	double angle = 0.0;									// 动画旋转角度
	SDL_Point *rotate_point = nullptr;					// 旋转中心

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
	~Animation()
	{
		if (rotate_point) delete rotate_point;
	}

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

	void set_rotate_center(int x, int y)
	{
		if (nullptr == rotate_point)
		{
			rotate_point = new SDL_Point({ -1, -1 });
		}
		rotate_point->x = x;
		rotate_point->y = y;
	}

	void on_update(float delta)
	{
		timer.on_update(delta);
	}

	void on_render(SDL_Renderer* renderer , const Camera& camera) 
	{
		const Frame& frame = frame_list[idx_frame];

		SDL_Rect rect_dst;
		rect_dst.w = frame.rect_src.w, rect_dst.h = frame.rect_src.h;
		rect_dst.x = (int)position.x - frame.rect_src.w / 2 - (int)camera.get_position().x;		// 中心锚点模式
		rect_dst.y = (int)position.y - frame.rect_src.h / 2 - (int)camera.get_position().x;

		SDL_RenderCopyEx(renderer, frame.tex_img, &frame.rect_src, &rect_dst, angle, rotate_point, SDL_FLIP_NONE);
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

	void add_frame(Atlas* atlas)
	{
		for (int i = 0; i < atlas->get_size(); ++i)
		{
			SDL_Texture* tex_img = atlas->get_texture(i);
			SDL_Rect rect_src;
			rect_src.x = 0, rect_src.y = 0;
			SDL_QueryTexture(tex_img, nullptr, nullptr, &rect_src.w, &rect_src.h);

			frame_list.emplace_back(tex_img, rect_src);
		}
	}
};