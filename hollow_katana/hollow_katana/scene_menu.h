#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "audio_manager.h"

class SceneMenu : public Scene
{
private:
	// ҳ�沼�ֲ���		


public:
	SceneMenu()
	{
		// ������Դ
	}

	virtual void on_enter() override 
	{
		AudioManager::instance()->play_audio_ex(_T("bgm1"), true);
	}
	virtual void on_input(const ExMessage& msg) override 
	{
		if (msg.lbutton)
			SceneManager::instance()->switch_scene("game_scene_boss_hornet");
		else if(msg.rbutton)
			SceneManager::instance()->switch_scene("menu_scene_introduce");
	}
	virtual void on_update(float delta_time) override 
	{
	
	}
	virtual void on_render() override 
	{
		static IMAGE* background = ResourcesManager::instance()->find_image("menu");
		static Rect rect_dst;
		rect_dst.x = (getwidth() - rect_dst.w) / 2;
		rect_dst.y = (getheight() - rect_dst.h) / 2;
		rect_dst.w = background->getwidth(), rect_dst.h = background->getheight();

		setbkcolor(RGB(0, 0, 0));
		putimage_alpha_ex(background, &rect_dst);

		settextcolor(RGB(255, 255,255));
		outtextxy(getwidth() / 2 - textwidth(_T("��ʼ")) / 2, getheight() / 2, _T("��ʼ"));
		outtextxy(getwidth() / 2 - textwidth(_T("˵��")) / 2, getheight() / 2 + 50, _T("˵��"));
		outtextxy(getwidth() / 2 - textwidth(_T("���������[��ʼ],�Ҽ�[˵��]")) / 2, getheight() - 70, _T("���������[��ʼ],�Ҽ�[˵��]"));
	}
	virtual void on_exit() override 
	{
		AudioManager::instance()->stop_audio_ex(_T("bgm1"));
	}
};



class SceneMunuInctroduce : public Scene
{
private:
	// ҳ�沼�ֲ���		

public:
	SceneMunuInctroduce()
	{
		// ������Դ
	}

	virtual void on_enter() override 
	{
		AudioManager::instance()->play_audio_ex(_T("bgm1"), true);
	}
	virtual void on_input(const ExMessage& msg) override 
	{
		if(msg.message == WM_KEYDOWN || msg.lbutton || msg.rbutton)
			SceneManager::instance()->switch_scene("menu");
	}
	virtual void on_update(float delta_time) override {}
	virtual void on_render() override 
	{
		static IMAGE* background = ResourcesManager::instance()->find_image("menu_introduce");
		static Rect rect_dst;
		rect_dst.x = (getwidth() - rect_dst.w) / 2;
		rect_dst.y = (getheight() - rect_dst.h) / 2;
		rect_dst.w = background->getwidth(), rect_dst.h = background->getheight();

		setbkcolor(RGB(0, 0, 0));
		putimage_alpha_ex(background, &rect_dst);
	}
	virtual void on_exit() override 
	{
		AudioManager::instance()->stop_audio_ex(_T("bgm1"));
	}
};