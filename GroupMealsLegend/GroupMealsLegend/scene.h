#pragma once
#include "SDL.h"


// 包含关系：场景 - 多个区域

class Scene
{
public:
	virtual void on_input(const SDL_Event& evnet) = 0;
	virtual void on_update(float delta) = 0;
	virtual void on_render(SDL_Renderer* renderer) = 0;
	virtual void on_enter() {};	// 场景切换资源准备工作
	virtual void on_exit() {};
};