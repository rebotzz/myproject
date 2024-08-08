#pragma once
#include "scene.h"
#include "scene_manager.h"
#include <iostream>

using std::cout;
using std::endl;

extern SceneManager scene_manager;


class GameScene : public Scene
{
private:

public:
	GameScene() = default;
	~GameScene() = default;


	virtual void on_enter()
	{
		cout << "进入游戏场景" << endl;
	}
	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			cout << "游戏场景处理按键消息" << endl;
			scene_manager.switch_scene(SceneManager::SceneType::MENU);

		}

	}
	virtual void on_update(int delta_time)
	{
		cout << "游戏场景正在运行, update" << endl;

	}
	virtual void on_draw()
	{
		outtextxy(0, 0, L"游戏界面");
	}
	virtual void on_exit()
	{
		cout << "游戏场景退出" << endl;

	}
};