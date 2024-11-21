#pragma once
#include "atlas.h"
#include "vector2.h"
#include "timer.h"
#include "camera.h"

// ��SDL_Surface����SDL_Texture
//
// v0.1:�����״���ȾʱSDL_SurfaceתSDL_Texture, ֮���ճ�����������ʱ�ͷ�SDL_Texture
// v0.2:����SDL_Texture��Ƶ������/����̫������,ͨ��SDL_QueryTexture�鿴������Ϣ

// ������
class Animation
{
private:
	// ����֡
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
	Timer timer;										// �������ŷ���ʱ��		
	Vector2 position;									// ����λ��(����ê��)
	size_t idx_frame = 0;								// ֡����,��ǰ���ڲ��ŵĶ���֡
	bool is_loop = false;								// �Ƿ�ѭ������					
	std::vector<Frame> frame_list;						// ����֡����
	std::function<void()> on_finished;					// ������������	
	double angle = 0.0;									// ������ת�Ƕ�
	SDL_Point *rotate_point = nullptr;					// ��ת����

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
		rect_dst.x = (int)position.x - frame.rect_src.w / 2 - (int)camera.get_position().x;		// ����ê��ģʽ
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