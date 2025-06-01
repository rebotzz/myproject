#pragma once
#include <easyx.h>

// ³¡¾°»ùÀà
class Scene
{
public:
	Scene() = default;
	virtual ~Scene() {};
	virtual void on_enter() = 0;
	virtual void on_input(const ExMessage& msg) { };
	virtual void on_update(float delta_time) { };
	virtual void on_render() { };
	virtual void on_exit() { };
};


