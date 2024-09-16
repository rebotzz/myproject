#pragma once
#include "util.h"
#include "scene.h"
#include "scene_manager.h"
#include "audio_manager.h"
#include "resources_manager.h"
#include "animation.h"

class SceneMenu : public Scene
{
private:
	enum class Choice
	{
		Game, Introduce, None
	};
private:
	// 页面布局参数		
	Animation animation_ui_choose_left;
	Animation animation_ui_choose_right;
	Animation animation_bug[5];
	Vector2 postion_bug[5];
	Timer timer;
	Choice choice = Choice::Game;
	float position_ui_y = 0.f;

public:
	SceneMenu()
	{
		animation_ui_choose_left.set_interval(0.1f);
		animation_ui_choose_left.set_loop(false);
		animation_ui_choose_left.set_achor_mode(Animation::AchorMode::Centered);
		animation_ui_choose_left.add_frame(ResourcesManager::instance()->find_atlas("ui_choose_left"));
		
		animation_ui_choose_right.set_interval(0.1f);
		animation_ui_choose_right.set_loop(false);
		animation_ui_choose_right.set_achor_mode(Animation::AchorMode::Centered);
		animation_ui_choose_right.add_frame(ResourcesManager::instance()->find_atlas("ui_choose_right"));
	
		for (int i = 0; i < 5; ++i)
		{
			animation_bug[i].set_interval(0.07f + random_range(0, 10) * 0.01f);
			animation_bug[i].set_loop(true);
			animation_bug[i].set_achor_mode(Animation::AchorMode::Centered);
			animation_bug[i].add_frame(ResourcesManager::instance()->find_atlas("light_bug"));
		}
		
		postion_bug[0] = { (float)getwidth() - 130.f, 240.f };
		postion_bug[1] = { (float)getwidth() - 240.f, 170.f };
		postion_bug[2] = { (float)getwidth() - 430.f, 280.f };
		postion_bug[3] = { (float)getwidth() - 140.f, (float)getheight() - 420.f };
		postion_bug[4] = { (float)getwidth() - 80.f, (float)getheight() - 190.f };

		timer.set_one_shot(false);
		timer.set_wait_time(0.2f);
		timer.set_on_timeout([&]
			{
				for (int i = 0; i < 5; ++i)
				{
					auto& bug = animation_bug[i];
					Vector2 pos_offset;
					float x = (float)random_range(0, 20);
					float y = (float)random_range(0, 20);
					if (random_range(0, 1))
					{
						x = -x;
						y = -y;
					}
					bug.set_position(postion_bug[i] + pos_offset);
				}
			});
	}

	virtual void on_enter() override 
	{
		AudioManager::instance()->play_audio_ex(_T("bgm_0"), true);
	}
	virtual void on_input(const ExMessage& msg) override 
	{
		static const int VK_W = 0x57;
		static const int VK_S = 0x53;

		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case VK_W:
				choice = (Choice)(((int)choice - 1 + (int)Choice::None) % (int)Choice::None);
				break;
			case VK_UP:
				choice = (Choice)(((int)choice - 1 + (int)Choice::None) % (int)Choice::None);
				break;
			case VK_S:
				choice = (Choice)(((int)choice + 1 + (int)Choice::None) % (int)Choice::None);
				break;
			case VK_DOWN:
				choice = (Choice)(((int)choice + 1 + (int)Choice::None) % (int)Choice::None);
				break;
			case VK_RETURN:
				switch (choice)
				{
				case Choice::Game:
				{
					AudioManager::instance()->stop_audio_ex(_T("bgm_0"));
					SceneManager::instance()->switch_scene("begin_scene");
					break;
				}
				case Choice::Introduce:
					SceneManager::instance()->switch_scene("menu_scene_introduce");
					break;
				}
				AudioManager::instance()->play_audio_ex(_T("ui_confirm"));
				break;
			}
			animation_ui_choose_left.reset();
			animation_ui_choose_right.reset();
			AudioManager::instance()->play_audio_ex(_T("ui_switch"));
			break;
		}
	}
	virtual void on_update(float delta) override 
	{
		animation_ui_choose_left.on_update(delta);
		animation_ui_choose_right.on_update(delta);

		timer.on_update(delta);
		for (auto& bug : animation_bug)
			bug.on_update(delta);
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

		static float level_gap = 160;
		switch (choice)
		{
		case Choice::Game:
			animation_ui_choose_left.set_position({ (float)getwidth() / 2 - level_gap, (float)getheight() - 150 });
			animation_ui_choose_right.set_position({ (float)getwidth() / 2 + level_gap, (float)getheight() - 150 });
			break;
		case Choice::Introduce:
			animation_ui_choose_left.set_position({ (float)getwidth() / 2 - level_gap, (float)getheight() - 70 });
			animation_ui_choose_right.set_position({ (float)getwidth() / 2 + level_gap, (float)getheight() - 70 });
			break;
		}
		animation_ui_choose_left.on_render();
		animation_ui_choose_right.on_render();

		for (auto& bug : animation_bug)
			bug.on_render();
	}
	virtual void on_exit() override 
	{
	}
};

class SceneMenuInctroduce : public Scene
{
private:
	// 页面布局参数		

public:
	SceneMenuInctroduce()
	{
		// 加载资源
	}

	virtual void on_enter() override 
	{
	}
	virtual void on_input(const ExMessage& msg) override 
	{
		if(msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
			SceneManager::instance()->switch_scene("menu_scene");
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

		outtextxy(30, 30, _T("ESC退出"));
	}
	virtual void on_exit() override 
	{
	}
};



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
				if(!next_scene.empty())
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



// 起始场景
class SceneBegin : public Scene
{
private:
	Animation animation;
	Timer timer;
	bool is_finish_animation = false;

public:
	SceneBegin()
	{
		// 加载资源
		animation.add_frame(ResourcesManager::instance()->find_atlas("light"));
		animation.set_interval(0.15f);
		animation.set_loop(false);
		animation.set_achor_mode(Animation::AchorMode::Centered);
		animation.set_position({ (float)getwidth() / 2, (float)getheight() / 2 });
		animation.set_on_finished([&]()
			{
				is_finish_animation = true;
			});

		timer.set_one_shot(true);
		timer.set_wait_time(4.0f);
		timer.set_on_timeout([&]()
			{
				SceneTransition* transition = dynamic_cast<SceneTransition*>(
					SceneManager::instance()->get_transition_scene());
				transition->set_next_scene("game_scene_boss_hornet");
				SceneManager::instance()->switch_scene("transition_scene");
			});
	}

	virtual void on_enter() override
	{
		is_finish_animation = false;
		timer.restart();
	}
	virtual void on_input(const ExMessage& msg) override
	{}
	virtual void on_update(float delta_time) override
	{
		if(!is_finish_animation)
			animation.on_update(delta_time);
		else
			timer.on_update(delta_time);
	}
	virtual void on_render() override
	{
		if (!is_finish_animation)
			animation.on_render();
		else
		{
			settextcolor(RGB(255, 255, 255));
			outtextxy(getwidth() / 2 - textwidth(_T("那么,再和我说说那个人吧...")) / 2, 
				getheight() / 2 - textheight(_T("那么,再和我说说那个人吧...")) / 2, 
				_T("那么,再和我说说那个人吧..."));
		}
	}
	virtual void on_exit() override
	{
	}
};

