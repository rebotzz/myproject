#pragma once
#include "scene.h"
#include "mine_board.h"
#include <memory>
#include <ctime>

extern int grid_x;
extern int grid_y;
extern int mine_count;
extern const int status_h;
extern TTF_Font* font;
extern SDL_Window* window;

class GameScene : public Scene
{
private:
	std::shared_ptr<MineBoard> mine_board = nullptr;	// 棋盘
	int x = 0, y = 0;									// 扫雷坐标
	bool is_sweep = true;								// 执行操作：扫雷/标记
	bool debug_show = false;							// debug展示所有地雷
	int begin_time = 0;									// 开始时间
	int timer = 0;										// 计时器
	Button button;										// 重置按键
	bool clickdown = false;								// 鼠标是否点击

public:
	GameScene()
		:button((grid_size* grid_x - 50) / 2, (status_h - 50) / 2, 50, 50)
	{
		button.set_on_click([this]()
			{
				on_enter();
			});
		button.set_on_draw([&](SDL_Renderer* renderer)
			{
				if(mine_board->is_alive())
					draw_smiling_face(renderer, (grid_size * grid_x - 50) / 2, (status_h - 50) / 2, 50);
				else
					draw_crying_face(renderer, (grid_size * grid_x - 50) / 2, (status_h - 50) / 2, 50);
			}, 
			[](SDL_Renderer* renderer)
			{
				draw_face(renderer, (grid_size * grid_x - 50) / 2, (status_h - 50) / 2, 50);
			});
	}

	virtual void on_enter() override
	{
		mine_board = std::shared_ptr<MineBoard>(new MineBoard(grid_x, grid_y, mine_count));
		begin_time = time(nullptr);
		SDL_SetWindowSize(window, grid_x * grid_size, grid_y * grid_size + status_h);
		SDL_Rect rect_screen;
		SDL_GetDisplayBounds(0, &rect_screen);
		SDL_SetWindowPosition(window, (rect_screen.w - grid_x * grid_size) / 2, 60);
		button.set_rect((grid_size * grid_x - 50) / 2, (status_h - 50) / 2, 50, 50);	// debug:菜单选择难度后棋盘大小改变
	}


	virtual void on_exit() override
	{
		mine_board = nullptr;
	}

	virtual void on_input(const SDL_Event& event) override
	{
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			// 鼠标左键扫雷，右键标记
			clickdown = true;
			is_sweep = (event.button.button == SDL_BUTTON_LEFT);
			x = event.button.x;
			y = event.button.y;
			break;
		case SDL_MOUSEBUTTONUP:
			clickdown = false;
			if (mine_board->is_alive() && !mine_board->is_seccess())
			{
				if (is_sweep)
					mine_board->sweep_point(x / grid_size, (y - status_h) / grid_size);
				else
					mine_board->flag_point(x / grid_size, (y - status_h) / grid_size);
			}

			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_q)
				debug_show = !debug_show;
			break;
		}
	}

	virtual void on_update(double delta) override
	{
		button.on_update(x, y, clickdown);
		if(!mine_board->is_seccess() && mine_board->is_alive())
			timer = time(nullptr) - begin_time;
	}

	virtual void on_render(SDL_Renderer* renderer) override
	{
		// 状态栏
		static SDL_Rect rect_mine_count = { 0, status_h * 0.2, 100, status_h * 0.6 };
		static SDL_Rect rect_time = { grid_size * grid_x - 100, status_h * 0.2 , 100, status_h * 0.6 };
		static SDL_Rect rect_status = { 0, 0, grid_size * grid_x, status_h };
		static SDL_Rect rect_mine = { 5, (status_h - grid_size / 2) / 2 , grid_size * 0.5, grid_size * 0.5 };
		static SDL_Color color_text = { 220, 50, 50, 255 };

		SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255);
		SDL_RenderFillRect(renderer, &rect_status);
		button.on_render(renderer);

		SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
		SDL_RenderFillRect(renderer, &rect_mine);
		draw_text(renderer, font, rect_mine_count.x + rect_mine_count.w / 2, rect_mine_count.y + rect_mine_count.h / 2,
			(const char*)std::to_string(mine_board->get_mine_count()).c_str(), color_text);
		draw_text(renderer, font, rect_time.x + rect_time.w / 2, rect_time.y + rect_time.h / 2,
			(const char*)std::to_string(timer).c_str(), color_text);
		if (mine_board->is_seccess())
		{
			draw_text(renderer, font, rect_time.x - 100, rect_time.y + rect_time.h / 2,
				(const char*)"Win !", color_text);
		}

		// 地雷棋盘
		if (!debug_show)
			mine_board->on_render(renderer);
		else
			mine_board->show_mine(renderer);
	}
};