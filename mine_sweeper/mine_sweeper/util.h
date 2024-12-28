#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <functional>
#include <string>


inline void draw_text(SDL_Renderer* renderer, TTF_Font* font, int center_x, int center_y, const char* text, SDL_Color color)
{
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, text, color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
	SDL_Rect rect_text = { center_x - suf_text->w / 2, center_y - suf_text->h / 2, suf_text->w, suf_text->h };
	SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
	SDL_DestroyTexture(tex_text);
	SDL_FreeSurface(suf_text);
}

inline void draw_text_ex(SDL_Renderer* renderer, TTF_Font* font, int left_x, int buttom_y, const char* text, SDL_Color color, bool subline = false)
{
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, text, color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
	SDL_Rect rect_text = { left_x, buttom_y - suf_text->h - 3, suf_text->w, suf_text->h };
	SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
	SDL_DestroyTexture(tex_text);
	SDL_FreeSurface(suf_text);
	if (subline)
	{
		SDL_RenderDrawLine(renderer, left_x, buttom_y, left_x + suf_text->w, buttom_y);
	}
}

inline void draw_circle(SDL_Renderer* renderer, int x, int y, int radius)
{
	for (int i = 0; i < 360; ++i)
	{
		SDL_RenderDrawPoint(renderer, x + radius * cos(i), y + radius * sin(i));
	}
}

inline void draw_fill_circle(SDL_Renderer* renderer, int x, int y, int radius)
{
	for (int i = 0; i < 360; ++i)
	{
		for (int r = 0; r <= radius; ++r)
		{
			SDL_RenderDrawPoint(renderer, x + r * cos(i), y + r * sin(i));
		}
	}
}

inline void draw_arc(SDL_Renderer* renderer, int x, int y, int radius, int begin_angle, int end_angle)
{
	for (int i = begin_angle; i <= end_angle; ++i)
	{
		double angle = 3.1415926 / 180.0 * i;
		SDL_RenderDrawPoint(renderer, x + radius * cos(angle), y + radius * sin(angle));
	}
}

inline void draw_flag(SDL_Renderer* renderer, int x, int y, int grid_size)
{
	SDL_Rect rect_flag = { x + grid_size * 0.45, y + grid_size * 0.2, grid_size * 0.4, grid_size * 0.3 };
	SDL_Rect rect_pole = { x + grid_size * 0.45, y + grid_size * 0.2, grid_size * 0.05, grid_size * 0.6 };
	SDL_Rect rect_bottom = { x + grid_size * 0.25, y + grid_size * 0.8, grid_size * 0.5, grid_size * 0.15 };

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect_flag);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, &rect_pole);
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderFillRect(renderer, &rect_bottom);
}

inline void draw_face(SDL_Renderer* renderer, int x, int y, int grid_size)
{
	SDL_Rect rect_l_eye = { x + grid_size * 0.1, y + grid_size * 0.3, grid_size * 0.3, grid_size * 0.2 };
	SDL_Rect rect_r_eye = { x + grid_size * 0.6, y + grid_size * 0.3, grid_size * 0.3, grid_size * 0.2 };
	SDL_Rect rect_mouth = { x + grid_size * 0.4, y + grid_size * 0.8, grid_size * 0.2, grid_size * 0.1 };
	SDL_Rect rect_bg = { x, y, grid_size, grid_size };

	SDL_SetRenderDrawColor(renderer, 127, 255, 212, 255);
	SDL_RenderFillRect(renderer, &rect_bg);
	SDL_SetRenderDrawColor(renderer, 72, 118, 255, 255);
	SDL_RenderFillRect(renderer, &rect_l_eye);
	SDL_RenderFillRect(renderer, &rect_r_eye);
	SDL_RenderFillRect(renderer, &rect_mouth);
}

inline void draw_smiling_face(SDL_Renderer* renderer, int x, int y, int grid_size)
{
	SDL_Rect rect_l_eye = { x + grid_size * 0.1, y + grid_size * 0.3, grid_size * 0.3, grid_size * 0.2 };
	SDL_Rect rect_r_eye = { x + grid_size * 0.6, y + grid_size * 0.3, grid_size * 0.3, grid_size * 0.2 };
	SDL_Rect rect_bg = { x, y, grid_size, grid_size };

	SDL_SetRenderDrawColor(renderer, 127, 255, 212, 255);
	SDL_RenderFillRect(renderer, &rect_bg);
	SDL_SetRenderDrawColor(renderer, 72, 118, 255, 255);
	SDL_RenderFillRect(renderer, &rect_l_eye);
	SDL_RenderFillRect(renderer, &rect_r_eye);
	for (int i = 0; i < 5; ++i)
	{
		draw_arc(renderer, x + grid_size / 2, y + grid_size / 2 + i, 15, 45, 135);
	}
}

inline void draw_crying_face(SDL_Renderer* renderer, int x, int y, int grid_size)
{
	SDL_Rect rect_l_eye = { x + grid_size * 0.1, y + grid_size * 0.3, grid_size * 0.3, grid_size * 0.2 };
	SDL_Rect rect_r_eye = { x + grid_size * 0.6, y + grid_size * 0.3, grid_size * 0.3, grid_size * 0.2 };
	SDL_Rect rect_bg = { x, y, grid_size, grid_size };

	SDL_SetRenderDrawColor(renderer, 127, 255, 212, 255);
	SDL_RenderFillRect(renderer, &rect_bg);
	SDL_SetRenderDrawColor(renderer, 72, 118, 255, 255);
	SDL_RenderFillRect(renderer, &rect_l_eye);
	SDL_RenderFillRect(renderer, &rect_r_eye);

	for (int i = 0; i < 5; ++i)
	{
		draw_arc(renderer, x + grid_size / 2, y + grid_size + i, 15, 225, 315);
	}
}


class Button
{
	enum class Status
	{
		Click, Idle
	};
private:
	SDL_Rect rect;
	Status status = Status::Idle;
	std::function<void()> on_click;
	std::function<void(SDL_Renderer*)> draw_idle;
	std::function<void(SDL_Renderer*)> draw_click;

public:
	Button(int x, int y, int w, int h)
	{
		rect.x = x, rect.y = y;
		rect.w = w, rect.h = h;
	}

	void set_on_click(const std::function<void()>& callback)
	{
		on_click = callback;
	}

	void set_on_draw(const std::function<void(SDL_Renderer*)>& _draw_idle,
		const std::function<void(SDL_Renderer*)>& _draw_click)
	{
		draw_idle = _draw_idle;
		draw_click = _draw_click;
	}

	void set_rect(int x, int y, int w, int h)
	{
		rect.x = x, rect.y = y;
		rect.w = w, rect.h = h;
	}

	void on_update(int cursor_x, int cursor_y, bool clickdown)
	{
		SDL_Point point = { cursor_x, cursor_y };

		if (clickdown && SDL_PointInRect(&point, &rect) && Status::Idle == status)
		{
			status = Status::Click;
		}
		else if (!clickdown && Status::Click == status)
		{
			status = Status::Idle;
			if (on_click)
				on_click();
		}
	}

	void on_render(SDL_Renderer* renderer)
	{
		switch (status)
		{
		case Status::Idle:
			if (draw_idle) draw_idle(renderer);
			break;
		case Status::Click:
			if (draw_click) draw_click(renderer);
			break;
		}
	}
};

class InputBlank
{
private:
	enum class Status
	{
		Input, Idle
	};
private:
	SDL_Rect rect;
	Status status = Status::Idle;
	std::string text;					// 输入文本
	std::string desc;					// 输入框描述
	int flash_counter = 0;				// 光标闪回计数器

public:
	void set_rect(int x, int y, int w, int h)
	{
		rect.x = x, rect.y = y;
		rect.w = w, rect.h = h;
	}

	void set_rect(const SDL_Rect& _rect)
	{
		rect = _rect;
	}

	// 默认值
	void set_text(const std::string& _text)
	{
		text = _text;
	}

	const std::string& get_text() const
	{
		return text;
	}

	void set_desc(const std::string& _desc)
	{
		desc = _desc;
	}

	void on_input(const SDL_Event& event)
	{
		if (Status::Input == status)
		{
			switch (event.type)
			{
			case SDL_TEXTINPUT:
				text += event.text.text;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_BACKSPACE)
					text.pop_back();
				break;
			}
		}
	}

	void on_update(int cursor_x, int cursor_y, bool clickdown)
	{
		SDL_Point point = { cursor_x, cursor_y };

		if (clickdown)
		{
			if (SDL_PointInRect(&point, &rect))
			{
				status = Status::Input;
			}
			else
			{
				status = Status::Idle;
			}
		}
	}

	void on_render(SDL_Renderer* renderer, TTF_Font* font)
	{
		static SDL_Color color = { 0,0,0,255 };
		if (Status::Idle == status)
		{
			SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
			color = { 100, 100, 100, 255 };
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			color = { 0,0,0,255 };
		}

		std::string cursor;
		if (Status::Input == status && ++flash_counter % 35 == 0)
		{
			if (cursor.empty()) cursor = " |";
			else cursor.clear();
		}
		else
		{
			cursor.clear();
		}

		//SDL_RenderDrawRect(renderer, &rect);
		draw_text_ex(renderer, font, rect.x, rect.y + rect.h, (desc + text + cursor).c_str(), color);
	}

};