#pragma once
#include "scene.h"

class SceneMunuInctroduce : public Scene
{
private:
	// ҳ�沼�ֲ���		

public:
	SceneMunuInctroduce()
	{
		// ������Դ
	}

	virtual void on_enter() override {}
	virtual void on_input(const ExMessage& msg) override {}
	virtual void on_update(int delta_time) override {}
	virtual void on_render(const Camera& camera) override {}
	virtual void on_exit() override {}
};