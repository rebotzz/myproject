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
		cout << "����ѡ����ҳ���" << endl;
	}
	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			cout << "ѡ����ҳ�����������Ϣ" << endl;
			scene_manager.switch_scene(SceneManager::SceneType::GAME);
		}

	}
	virtual void on_update(int delta_time)
	{
		cout << "ѡ����ҳ�����������, update" << endl;

	}
	virtual void on_draw()
	{
		outtextxy(0, 0, L"ѡ����ҽ���");
	}
	virtual void on_exit()
	{
		cout << "ѡ����ҳ����˳�" << endl;

	}
};