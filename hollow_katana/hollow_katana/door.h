#pragma once
#include "scene_manager.h"
#include "interact_prop.h"
#include "timer.h"

// 传送门
class Door
{
private:
	std::string dst_scene;						// 目标场景
	InteractProp prop;							// 交互道具
	bool is_enter = false;						// 是否进入门,避免重复碰撞触发
	Timer timer_enter;

public:
	Door(const std::string& destination)
		:dst_scene(destination)
		,prop(_T("传送门"))
	{
		prop.set_on_interact([&]()
			{
				is_enter = true;
			});
		prop.enable_showing_box(true);
		prop.set_showing_style(RGB(0, 255, 255), PS_DASH, 3);

		timer_enter.set_one_shot(true);
		timer_enter.set_wait_time(0.5f);
		timer_enter.set_on_timeout([&]()
			{
				SceneManager::instance()->set_transition_next_scene(dst_scene);
				SceneManager::instance()->switch_scene("transition_scene");
			});
	}
	~Door()
	{}

	void set_color(COLORREF col)
	{
		prop.set_showing_style(col, PS_DASH, 3);
	}

	void set_describe(const std::wstring& desc)
	{
		prop.set_describe(desc);
	}

	void set_size(const Vector2& size)
	{
		prop.set_size(size);
	}

	// 中心锚点模式
	void set_position(const Vector2& position)
	{
		prop.set_position(position);
	}

	void on_update(float delta)
	{
		if (is_enter)
			timer_enter.on_update(delta);
	}

	void on_render()
	{
		prop.on_render();
	}

	void set_enabled(bool flag)
	{
		prop.set_enabled(flag);
	}
};