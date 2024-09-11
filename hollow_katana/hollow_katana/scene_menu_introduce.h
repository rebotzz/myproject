#pragma once
#include "scene.h"

class SceneMunuInctroduce : public Scene
{
private:
	// 页面布局参数		

public:
	SceneMunuInctroduce()
	{
		// 加载资源
	}

	virtual void on_enter() override {}
	virtual void on_input(const ExMessage& msg) override {}
	virtual void on_update(int delta_time) override {}
	virtual void on_render(const Camera& camera) override {}
	virtual void on_exit() override {}
};