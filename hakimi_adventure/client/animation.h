#pragma once
#include <vector>
#include <functional>
#include "util.h"
#include "atlas.h"
#include "vector2.h"
#include "timer.h"

class Animation
{
private:
	// ����֡
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
	Timer timer;										// �������ŷ���ʱ��		
	Vector2 position;									// ����λ��(����ê��)
	size_t idx_frame = 0;								// ֡����,��ǰ���ڲ��ŵĶ���֡
	bool is_loop = false;								// �Ƿ�ѭ������					
	std::vector<Frame> frame_list;						// ����֡����
	std::function<void()> on_finished;					// ������������	

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

	void on_update(float delta)
	{
		timer.on_update(delta);
	}

	void on_render(const Camera& camera) const
	{
		const Frame& frame = frame_list[idx_frame];

		Rect rect_dst;
		rect_dst.w = frame.rect_src.w, rect_dst.h = frame.rect_src.h;
		rect_dst.x = (int)position.x - frame.rect_src.w / 2;		// ����ê��ģʽ
		rect_dst.y = (int)position.y - frame.rect_src.h / 2;

		putimage_alpha_ex(camera, frame.img, &rect_dst, &frame.rect_src);
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

	void add_frame(IMAGE* img, const Rect& rect_src)
	{
		frame_list.push_back({ img, rect_src });
	}
};