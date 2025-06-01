#pragma once

#include "timer.h"
#include "vector2.h"

#include "SDL.h"
#include <vector>

class Animation
{
private:
	// ����֡
	struct Frame
	{
	public:
		SDL_Texture* tex = nullptr;
		SDL_Rect src_rect = { 0 };
	public:
		Frame(SDL_Texture* tex, const SDL_Rect& src_rect)
			:tex(tex), src_rect(src_rect){ }
	};
	typedef std::vector<Frame> FrameList;

public:
	// ����λ�õ�����ģʽ����������λ��/�ײ�����λ��
	enum class Mode
	{
		Center,
		BottomCenter
	};

public:
	Animation()
	{
		timer.set_one_shot(false);
		timer.set_on_timeout([this]()
			{
				idx++;
				if (idx >= frame_list.size())
				{
					idx = is_loop ? idx = 0 : idx = frame_list.size() - 1;
					if (!is_loop && on_finished)
						on_finished();
				}
			});
	}
	~Animation() = default;

	// ��һ��ͼƬ�ü�Ϊ������Σ�ѡȡһ������Ϊ����֡����(��0��ʼ�������ң����ϵ���)
	void set_frame(SDL_Texture* tex, int frame_w, int frame_h, const std::vector<int>& frame_idx_list)
	{
		int tex_width = 0, tex_height = 0;
		SDL_QueryTexture(tex, nullptr, nullptr, &tex_width, &tex_height);
		int num_x = tex_width / frame_w, num_y = tex_height / frame_h;

		SDL_Rect rect = { 0, 0, frame_w, frame_h };
		for (auto frame_idx : frame_idx_list)
		{
			rect.x = (frame_idx % num_x) * frame_w;
			rect.y = (frame_idx / num_x) * frame_h;
			frame_list.emplace_back(tex, rect);
		}
	}

	void set_position_mode(Mode mode)
	{
		this->position_mode = mode;
	}

	void set_loop(bool flag)
	{
		is_loop = flag;
	}

	void set_on_finished(const std::function<void()>& callback)
	{
		on_finished = callback;
	}

	void restart()
	{
		idx = 0;
		timer.restart();
	}

	void set_interval(float second)
	{
		timer.set_wait_time(second);
	}

	void set_angle(double angle)
	{
		this->angle = angle;
	}

	void set_position(const Vector2& pos)
	{
		position = pos;
	}

	void on_update(float delta)
	{
		timer.on_update(delta);
	}

	void on_render(SDL_Renderer* renderer)
	{
		Frame& frame = frame_list[idx];
		
		SDL_Rect dstrect = {
			position.x - frame.src_rect.w / 2,
			(position_mode == Mode::Center ? position.y - frame.src_rect.h / 2 : position.y - frame.src_rect.h),
			frame.src_rect.w,
			frame.src_rect.h
		};

		SDL_RenderCopyEx(renderer, frame.tex, &frame.src_rect, &dstrect, angle, nullptr, SDL_FLIP_NONE);
	}

private:
	int idx = 0;							// ��ǰ֡���
	FrameList frame_list;					// ֡����
	bool is_loop = false;					// �Ƿ�ѭ������
	Timer timer;							// ��ʱ��
	std::function<void()> on_finished;		// ���������ص��߼�
	Vector2 position;						// ����λ��
	double angle = 0;						// ��ת�Ƕ�
	Mode position_mode = Mode::Center;		// ����λ��ģʽ

};
