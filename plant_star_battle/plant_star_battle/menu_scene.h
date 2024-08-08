#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "atlas.h"
#include "animation.h"
#include "camera.h"
#include <iostream>

using std::cout;
using std::endl;

extern SceneManager scene_manager;
extern Atlas atlas_player_peashooter_run_right;

class MenuScene : public Scene
{
private:
	Animation _anim_peashooter_run_right;
	Camera _camera;

public:
	MenuScene() = default;
	~MenuScene() = default;


	virtual void on_enter() 
	{
		cout << "进入菜单场景" << endl;

		_camera.reset();
		_anim_peashooter_run_right.set_atlas(&atlas_player_peashooter_run_right);
		_anim_peashooter_run_right.set_interval(75);
		_anim_peashooter_run_right.set_loop(true);
		_anim_peashooter_run_right.set_callback(
			[]() {
			scene_manager.switch_scene(SceneManager::SceneType::GAME);
			}
		);
	}
	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			cout << "菜单场景处理按键消息" << endl;
			scene_manager.switch_scene(SceneManager::SceneType::SELECTOR);
		}

	}
	virtual void on_update(int delta_time)
	{
		cout << "菜单场景正在运行, update" << endl;
		_anim_peashooter_run_right.on_update(delta_time);
		_camera.on_update(delta_time);
	}
	virtual void on_draw() 
	{
		outtextxy(0, 0, L"主菜单界面");
		Vector2 pos_carema = _camera.get_position();
		_anim_peashooter_run_right.on_draw((int)(100 - pos_carema._x), (int)(100 - pos_carema._y));
	}
	virtual void on_exit() 	
	{
		cout << "菜单场景退出" << endl;

	}
};