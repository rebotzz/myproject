#pragma once
#include "scene.h"
#include "util.h"
#include "scene_manager.h"

extern int grid_x;
extern int grid_y;
extern int mine_count;
extern const int grid_size;
extern const int status_h;
extern TTF_Font* font;
extern SDL_Window* window;

class MenuScene : public Scene
{
private:
	const int window_w = 600;
	const int window_h = 800;
	const SDL_Rect rect_simple = { window_w / 2 - 40, (int)(window_h * 0.3), 80, 40 };		// 初始化列表按照这里的先后顺序
	const SDL_Rect rect_middle = { window_w / 2 - 40, (int)(window_h * 0.38), 80, 40 };
	const SDL_Rect rect_expert = { window_w / 2 - 40, (int)(window_h * 0.46), 80, 40 };
	const SDL_Rect rect_fullsreen = { window_w / 2 - 40, (int)(window_h * 0.54), 80, 40 };
	const SDL_Rect rect_userdefine = { window_w / 2 - 40, (int)(window_h * 0.62), 80, 40 };
	const SDL_Rect rect_input_gridx = { (int)(window_w * 0.1), (int)(window_h * 0.7), 80, 40 };
	const SDL_Rect rect_input_gridy = { (int)(window_w * 0.4), (int)(window_h * 0.7), 80, 40 };
	const SDL_Rect rect_input_mine = { (int)(window_w * 0.7), (int)(window_h * 0.7), 110, 40 };

	Button button_simple;
	Button button_middle;
	Button button_expert;
	Button button_fullsreen;
	Button button_userdefine;
	InputBlank input_gridx;
	InputBlank input_gridy;
	InputBlank input_mine;
	bool show_userdefine = false;
	bool clickdown = false;								// 鼠标是否点击
	int x = 0, y = 0;									// 鼠标位置

public:
	MenuScene()
		:button_simple(rect_simple.x, rect_simple.y, rect_simple.w, rect_simple.h),
		button_middle(rect_middle.x, rect_middle.y, rect_middle.w, rect_middle.h),
		button_expert(rect_expert.x, rect_expert.y, rect_expert.w, rect_expert.h),
		button_fullsreen(rect_fullsreen.x, rect_fullsreen.y, rect_fullsreen.w, rect_fullsreen.h),
		button_userdefine(rect_userdefine.x, rect_userdefine.y, rect_userdefine.w, rect_userdefine.h)
	{
		static const SDL_Color color_idle = { 65, 105, 225, 255 };
		static const SDL_Color color_click = { 0, 0, 205, 255 };

		button_simple.set_on_click([]
			{
				grid_x = grid_y = 9;
				mine_count = 10;
				SceneManager::instance()->switch_scene("game");
			});
		button_simple.set_on_draw([&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_simple.x + rect_simple.w / 2, rect_simple.y + rect_simple.h / 2, u8"简单", color_idle);
			},
			[&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_simple.x + rect_simple.w / 2, rect_simple.y + rect_simple.h / 2, u8"简单", color_click);
			});

		button_middle.set_on_click([]
			{
				grid_x = grid_y = 16;
				mine_count = 40;
				SceneManager::instance()->switch_scene("game");
			});
		button_middle.set_on_draw([&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_middle.x + rect_middle.w / 2, rect_middle.y + rect_middle.h / 2, u8"中等", color_idle);
			},
			[&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_middle.x + rect_middle.w / 2, rect_middle.y + rect_middle.h / 2, u8"中等", color_click);
			});

		button_expert.set_on_click([]
			{
				grid_x = 30, grid_y = 16;
				mine_count = 99;
				SceneManager::instance()->switch_scene("game");
			});
		button_expert.set_on_draw([&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_expert.x + rect_expert.w / 2, rect_expert.y + rect_expert.h / 2, u8"困难", color_idle);
			},
			[&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_expert.x + rect_expert.w / 2, rect_expert.y + rect_expert.h / 2, u8"困难", color_click);
			});

		button_fullsreen.set_on_click([]
			{
				SDL_Rect rect_screen;
				SDL_GetDisplayBounds(0, &rect_screen);
				grid_x = rect_screen.w / grid_size, grid_y = (rect_screen.h - status_h - 100) / grid_size;
				mine_count = grid_x * grid_y * 0.2;
				SceneManager::instance()->switch_scene("game");
			});
		button_fullsreen.set_on_draw([&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_fullsreen.x + rect_fullsreen.w / 2, rect_fullsreen.y + rect_fullsreen.h / 2, u8"全屏", color_idle);
			},
			[&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_fullsreen.x + rect_fullsreen.w / 2, rect_fullsreen.y + rect_fullsreen.h / 2, u8"全屏", color_click);
			});


		button_userdefine.set_on_click([&]
			{
				if (!show_userdefine)
				{
					show_userdefine = true;
				}
				else
				{
					grid_x = std::stoi(input_gridx.get_text());
					grid_y = std::stoi(input_gridy.get_text());
					mine_count = std::stoi(input_mine.get_text());
					SceneManager::instance()->switch_scene("game");
				}
			});
		button_userdefine.set_on_draw([&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_userdefine.x + rect_userdefine.w / 2, rect_userdefine.y + rect_userdefine.h / 2, u8"自定义", color_idle);
			},
			[&](SDL_Renderer* renderer)
			{
				draw_text(renderer, font, rect_userdefine.x + rect_userdefine.w / 2, rect_userdefine.y + rect_userdefine.h / 2, u8"自定义", color_click);
			});

		input_gridx.set_desc(u8"横:");
		input_gridy.set_desc(u8"竖:");
		input_mine.set_desc(u8"地雷:");
		input_gridx.set_rect(rect_input_gridx);
		input_gridy.set_rect(rect_input_gridy);
		input_mine.set_rect(rect_input_mine);
		input_gridx.set_text("10");
		input_gridy.set_text("10");
		input_mine.set_text("15");
	}


	~MenuScene()
	{
		//SDL_Log("~MenuScene()\n");
	}

	virtual void on_enter() override
	{
		SDL_SetWindowSize(window, window_w, window_h);
	}

	virtual void on_exit() override
	{
		SceneManager::instance()->remove_scene("menu");
	}

	virtual void on_input(const SDL_Event& event) override
	{
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			// 鼠标左键扫雷，右键标记
			clickdown = true;
			x = event.button.x;
			y = event.button.y;
			break;
		case SDL_MOUSEBUTTONUP:
			clickdown = false;
			break;
		}

		if (show_userdefine)
		{
			input_gridx.on_input(event);
			input_gridy.on_input(event);
			input_mine.on_input(event);
		}
	}

	virtual void on_update(double delta) override
	{
		button_simple.on_update(x, y, clickdown);
		button_middle.on_update(x, y, clickdown);
		button_expert.on_update(x, y, clickdown);
		button_fullsreen.on_update(x, y, clickdown);
		button_userdefine.on_update(x, y, clickdown);

		if (show_userdefine)
		{
			input_gridx.on_update(x, y, clickdown);
			input_gridy.on_update(x, y, clickdown);
			input_mine.on_update(x, y, clickdown);
		}
	}

	virtual void on_render(SDL_Renderer* renderer) override
	{
		static SDL_Color color_text = { 100, 100,100, 255 };
		SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
		SDL_RenderClear(renderer);
		draw_text(renderer, font, window_w / 2, window_h * 0.2, u8"《扫雷》", color_text);

		button_simple.on_render(renderer);
		button_middle.on_render(renderer);
		button_expert.on_render(renderer);
		button_fullsreen.on_render(renderer);
		button_userdefine.on_render(renderer);

		if (show_userdefine)
		{
			input_gridx.on_render(renderer, font);
			input_gridy.on_render(renderer, font);
			input_mine.on_render(renderer, font);
		}

	}
};
