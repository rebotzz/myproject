#pragma once
#include <easyx.h>
#include "camera.h"

// ³¡¾°»ùÀà
class Scene
{
public:
	Scene() = default;
	~Scene() = default;
	virtual void on_enter() = 0;
	virtual void on_input(const ExMessage& msg) = 0;
	virtual void on_update(float delta_time) = 0;
	virtual void on_render() = 0;
	virtual void on_exit() = 0;
};