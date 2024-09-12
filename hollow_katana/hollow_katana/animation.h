#pragma once
#include <vector>
#include <functional>
#include "util.h"
#include "atlas.h"
#include "vector2.h"
#include "timer.h"

class Animation
{
public:
	// 动画的锚点模式
	enum class AchorMode
	{
		Centered,
		BottomCentered
	};

private:
	// 动画帧
	struct Frame
	{
		Rect rect_src;
		IMAGE* img = nullptr;

		Frame() = default;
		Frame(IMAGE* img, Rect rect)
			:img(img), rect_src(rect) {}
		~Frame() = default;
	};

private:
	Timer timer;										// 动画播放发计时器		
	Vector2 position;
	size_t idx_frame = 0;								// 帧索引,当前正在播放的动画帧
	bool is_loop = false;								// 是否循环播放					
	std::vector<Frame> frame_list;						// 动画帧序列
	std::function<void()> on_finished;					// 动画结束处理	
	AchorMode achor_mode = AchorMode::Centered;			// 动画锚点

	// 其余
	IMAGE current_frame;								// 当前帧图片,用于生成粒子特效

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

	void set_achor_mode(AchorMode mode)
	{
		achor_mode = mode;
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

	void on_update(float delta)
	{
		timer.on_update(delta);
	}

	void on_render() const
	{
		if(idx_frame >= frame_list.size() || nullptr == frame_list[idx_frame].img)
		{
			return;

			// throw std::invalid_argument("render frame idx out of range, or is nullptr");
		}

		const Frame& frame = frame_list[idx_frame];

		Rect rect_dst;
		rect_dst.w = frame.rect_src.w, rect_dst.h = frame.rect_src.h;
		rect_dst.x = (int)position.x - frame.rect_src.w / 2;
		rect_dst.y = (achor_mode == AchorMode::BottomCentered ?
			(int)position.y - frame.rect_src.h :
			(int)position.y - frame.rect_src.h / 2);

		putimage_alpha_ex(frame.img, &rect_dst, &frame.rect_src);
	}

	void add_frame(IMAGE* img, int num_w)
	{
		int height = img->getheight();
		int width = img->getwidth();
		int width_frame = width / num_w;

		for (int i = 0; i < num_w; ++i)
		{
			Rect rect_src;
			rect_src.x = width_frame * i, rect_src.y = 0;
			rect_src.w = width_frame, rect_src.h = height;

			frame_list.emplace_back(img, rect_src);
		}
	}

	void add_frame(Atlas* atlas)
	{
		for (int i = 0; i < atlas->get_size(); ++i)
		{
			IMAGE* img = atlas->get_image(i);
			Rect rect_src;
			rect_src.x = 0, rect_src.y = 0;
			rect_src.w = img->getwidth(), rect_src.h = img->getheight();

			frame_list.emplace_back(img, rect_src);
		}
	}

	IMAGE& get_current_frame()
	{
		Frame frame = frame_list[idx_frame];
		int w_total = frame.img->getwidth();
		int w = frame.rect_src.w;
		int h = frame.rect_src.h;
		int x_pos = frame.rect_src.x;
		int y_pos = frame.rect_src.y;
		current_frame.Resize(w, h);

		DWORD* src_buffer = GetImageBuffer(frame.img);
		DWORD* dst_buffer = GetImageBuffer(&current_frame);

		for (int y = y_pos; y < y_pos + h; ++y)
		{
			for (int x = x_pos; x < x_pos + w; ++x)
			{
				int idx_src = y * w_total + x;
				int idx_dst = (y - y_pos) * w + (x - x_pos);
				dst_buffer[idx_dst] = src_buffer[idx_src];
			}
		}

		return current_frame;
	}

	void clear()
	{
		idx_frame = 0;
		frame_list.clear();
	}

	bool empty()
	{
		return frame_list.empty();
	}


	// todo: 可能没用
	void finish()
	{
		timer.finish();
	}
};