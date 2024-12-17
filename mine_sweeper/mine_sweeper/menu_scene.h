#pragma once
#include "scene.h"

//extern int grid_x;
//extern int grid_y;
//extern int mine_count;

class MenuScene : public Scene
{
private:
	enum class Mode		// 难度模式
	{
		Simple,
		Middle,
		Expert,
		FullSreen
	};
private:

public:
	virtual void on_enter()
	{}

	virtual void on_exit()
	{}

	virtual void on_input(const SDL_Event& event) override
	{}

	virtual void on_update(double delta) override
	{}

	virtual void on_render(SDL_Renderer* renderer) override
	{}
};
