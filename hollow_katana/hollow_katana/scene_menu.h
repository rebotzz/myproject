#pragma once
#include "scene.h"

class SceneMenu : public Scene
{
private:
	// ҳ�沼�ֲ���		

public:
	SceneMenu()
	{
		// ������Դ
	}

	virtual void on_enter() override {}
	virtual void on_input(const ExMessage& msg) override {}
	virtual void on_update(float delta_time) override {}
	virtual void on_render() override {}
	virtual void on_exit() override {}
};