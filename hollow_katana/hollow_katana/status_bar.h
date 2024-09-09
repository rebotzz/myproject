#pragma once
#include "easyx.h"

class StatusBar
{
private:
	IMAGE* ui_heart = nullptr;
	int hp = 0;
	float current_mp_ratio = 1.0f;


public:
	StatusBar();
	~StatusBar() = default;
	void on_update(float delta);
	void on_render();
};