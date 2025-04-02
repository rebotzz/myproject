#pragma once
#include "SDL.h"


// ������ϵ������ - �������

class Scene
{
public:
	virtual void on_input(const SDL_Event& evnet) = 0;
	virtual void on_update(float delta) = 0;
	virtual void on_render(SDL_Renderer* renderer) = 0;
	virtual void on_enter() {};	// �����л���Դ׼������
	virtual void on_exit() {};
};