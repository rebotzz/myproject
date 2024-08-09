#pragma once
#include <easyx.h>
#include "camera.h"

// ³¡¾°»ùÀà
class Scene
{
public:
	Scene() = default;
	~Scene() = default;
	virtual void on_enter() {}
	virtual void on_input(const ExMessage& msg) {}
	virtual void on_update(int delta_time) {}
	virtual void on_draw(const Camera& camera) {}
	virtual void on_exit(){}
};