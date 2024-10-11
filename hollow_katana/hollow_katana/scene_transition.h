#pragma once
#include "util.h"
#include "scene.h"
#include "animation.h"
#include "audio_manager.h"
#include "resources_manager.h"

// 过度场景
class SceneTransition : public Scene
{
private:
	enum class Status
	{
		Enter, Exit
	};
private:
	// todo: 或许淡入淡出效果在别的场景切出的时候也需要,放在场景管理器中吗?
	const float SPEED_PROGRESS = 2.f;					// 进入速度 1/3s
	float progress = 0.0f;								// 进度[0, 1]
	Status status = Status::Exit;
	Timer timer_fade_out;

private:
	Animation animation;
	std::string next_scene;
	Timer timer_exit;

public:
	SceneTransition()
	{
		// 加载资源
		animation.add_frame(ResourcesManager::instance()->find_atlas("little_knight"));
		animation.set_interval(0.1f);
		animation.set_loop(true);
		animation.set_achor_mode(Animation::AchorMode::Centered);
		animation.set_position({ (float)getwidth() / 2, (float)getheight() / 2 });

		float delta = 4.0f;
		timer_exit.set_one_shot(true);
		timer_exit.set_wait_time(delta);
		timer_exit.set_on_timeout([&]()
			{
				if (!next_scene.empty())
					SceneManager::instance()->switch_scene(next_scene);
				else
					SceneManager::instance()->switch_scene("menu_scene");
			});

		timer_fade_out.set_one_shot(true);
		timer_fade_out.set_wait_time(delta - 0.5f);
		timer_fade_out.set_on_timeout([&]
			{
				status = Status::Exit;
			});
	}

	void set_next_scene(std::string id)
	{
		next_scene = id;
	}

	virtual void on_enter() override
	{
		timer_exit.restart();
		timer_fade_out.restart();
		status = Status::Enter;
		progress = 0.0f;

		AudioManager::instance()->play_audio_ex(_T("play_tape"));
	}
	virtual void on_input(const ExMessage& msg) override
	{}
	virtual void on_update(float delta_time) override
	{
		animation.on_update(delta_time);
		timer_exit.on_update(delta_time);
		timer_fade_out.on_update(delta_time);

		progress += (status == Status::Enter ? SPEED_PROGRESS * delta_time : -SPEED_PROGRESS * delta_time);
		if (progress < 0.0)
			progress = 0.0;
		else if (progress > 1.0)
			progress = 1.0;
	}
	virtual void on_render() override
	{
		animation.on_render();
		post_progress();
	}
	virtual void on_exit() override
	{
	}

	// 全屏变暗的后处理特效
	void post_progress()
	{
		if (progress < 0.001f)
			return;

		DWORD* img_buffer = GetImageBuffer();
		int width = getwidth();
		int height = getheight();

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				int idx = y * width + x;
				COLORREF color = img_buffer[idx];
				BYTE r = (BYTE)(GetBValue(color) * lerp(0.0f, 1.0f, progress));
				BYTE g = (BYTE)(GetGValue(color) * lerp(0.0f, 1.0f, progress));
				BYTE b = (BYTE)(GetRValue(color) * lerp(0.0f, 1.0f, progress));
				img_buffer[idx] = BGR(RGB(r, g, b)) | ((DWORD)((BYTE)255) << 24);
			}
		}
	}

	float lerp(float start, float end, float progress)
	{
		return start + progress * (end - start);
	}
};