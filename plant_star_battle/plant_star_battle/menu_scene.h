#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "animation.h"
#include "camera.h"

extern SceneManager scene_manager;
extern IMAGE img_menu_background;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;


	virtual void on_enter() 
	{
		mciSendString(L"play bgm_menu repeat from 0", nullptr, 0, nullptr);
	}
	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYUP || msg.message == WM_LBUTTONUP)
		{
			mciSendString(L"play ui_confirm from 0", nullptr, 0, nullptr);
			scene_manager.switch_scene(SceneManager::SceneType::SELECTOR);
		}

	}
	virtual void on_update(int delta_time)
	{

	}
	virtual void on_draw(const Camera& camera)
	{
		putimage(0, 0, &img_menu_background);
	}
	virtual void on_exit() 	
	{
		mciSendString(L"stop bgm_menu", nullptr, 0, nullptr);
	}
};