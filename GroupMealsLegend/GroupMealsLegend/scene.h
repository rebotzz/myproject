#pragma once
#include "SDL.h"


// ������ϵ������ - �������

class Scene
{
public:
	virtual void on_input(const SDL_Event& evnet);
	virtual void on_update(float delta);
	virtual void on_render(SDL_Renderer* renderer);
	// �����л���Դ����/�ͷ�
	virtual void on_enter() = 0;	
	virtual void on_exit() {};
};