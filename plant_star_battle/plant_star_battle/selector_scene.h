#pragma once
#include "scene.h"
#include "scene_manager.h"
#include <iostream>

using std::cout;
using std::endl;

extern SceneManager scene_manager;


class SelectorScene : public Scene
{
private:

public:
	SelectorScene() = default;
	~SelectorScene() = default;


	virtual void on_enter()
	{
		cout << "进入选择玩家场景" << endl;
	}
	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			cout << "选择玩家场景处理按键消息" << endl;
			scene_manager.switch_scene(SceneManager::SceneType::GAME);
		}

	}
	virtual void on_update(int delta_time)
	{
		cout << "选择玩家场景正在运行, update" << endl;

	}
	virtual void on_draw()
	{
		outtextxy(0, 0, L"选择玩家界面");
	}
	virtual void on_exit()
	{
		cout << "选择玩家场景退出" << endl;

	}
};