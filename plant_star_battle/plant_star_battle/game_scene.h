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
		cout << "������Ϸ����" << endl;
	}
	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			cout << "��Ϸ������������Ϣ" << endl;
			scene_manager.switch_scene(SceneManager::SceneType::MENU);

		}

	}
	virtual void on_update(int delta_time)
	{
		cout << "��Ϸ������������, update" << endl;

	}
	virtual void on_draw()
	{
		outtextxy(0, 0, L"��Ϸ����");
	}
	virtual void on_exit()
	{
		cout << "��Ϸ�����˳�" << endl;

	}
};