#pragma once
#include <SDL.h>

class Scene
{
public:
	Scene() = default;
	~Scene() = default;
	virtual void on_enter()
	{}

	virtual void on_exit()
	{}

	virtual void on_input(const SDL_Event& event)
	{}

	virtual void on_update(double delta)
	{}

	virtual void on_render(SDL_Renderer* renderer) 
	{}
};





