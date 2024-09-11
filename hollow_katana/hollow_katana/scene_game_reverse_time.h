#pragma once
#include "scene.h"

class SceneGameReverseTime : public Scene
{
private:
	

public:

	// 只用进入开启回溯时间管理器就行,回溯完成跳转上一个游戏场景

	virtual void on_enter() override {}
	virtual void on_input(const ExMessage& msg) override {}
	virtual void on_update(int delta_time) override {}
	virtual void on_render(const Camera& camera) override {}
	virtual void on_exit() override {}
};