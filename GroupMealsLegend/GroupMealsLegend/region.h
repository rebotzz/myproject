#pragma once
#include "SDL.h"
#include <functional>

class Region
{
protected:
	SDL_Rect rect = { 0 };
	bool valid = true;

public:
	Region() = default;
	Region(const SDL_Rect& _rect) :rect(_rect) {};
	~Region() = default;

	virtual void on_update(float delta) {};
	virtual void on_render(SDL_Renderer* renderer) {};
	virtual void on_cursor_down() {};
	virtual void on_cursor_up() {};

	void set_rect(const SDL_Rect& rect)
	{
		this->rect = rect;
	}
	const SDL_Rect& get_rect() const
	{
		return rect;
	}

	void set_valid(bool flag)
	{
		valid = flag;
	}
	bool get_valid() const
	{
		return valid;
	}
};

class Region2 : public Region
{
private:
	std::function<void(SDL_Renderer*)> render_callback;

public:
	virtual void on_render(SDL_Renderer* renderer) 
	{
		if (render_callback)
			render_callback(renderer);
	};

	void set_render_callback(const std::function<void(SDL_Renderer*)>& callback)
	{
		render_callback = callback;
	}
};