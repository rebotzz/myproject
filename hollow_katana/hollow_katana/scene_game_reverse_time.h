#pragma once
#include "scene.h"
#include "reverse_time_manager.h"

class SceneGameReverseTime : public Scene
{
public:

	virtual void on_enter() override 
	{
		// ֻ�ý��뿪������ʱ�����������,���������ת��һ����Ϸ����
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

		// ���Ҫ�л����ĳ������ǻ���ʱ�䳡��,��ô�����ʷ֡��¼
		ReverseTimeManager::instance()->clear();
	}
};