#pragma once
#include "scene.h"
#include "reverse_time_manager.h"

class SceneGameReverseTime : public Scene
{
public:

	virtual void on_enter() override 
	{
		// 只用进入开启回溯时间管理器就行,回溯完成跳转上一个游戏场景
		ReverseTimeManager::instance()->reverse_time();
	}

	virtual void on_input(const ExMessage& msg) override 
	{}

	virtual void on_update(float delta) override 
	{
		ReverseTimeManager::instance()->on_update(delta);
	}
	virtual void on_render() override 
	{
		ReverseTimeManager::instance()->on_render();
	}
	virtual void on_exit() override 
	{
		ReverseTimeManager::instance()->set_enable(false);

		// 如果要切换到的场景不是回溯时间场景,那么清空历史帧记录
		ReverseTimeManager::instance()->clear();
	}
};