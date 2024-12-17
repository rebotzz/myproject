#pragma once
#include "scene.h"
#include "mine_board.h"
#include <memory>

extern int grid_x;
extern int grid_y;
extern int mine_count;
extern const int status_h;

class GameScene : public Scene
{
private:
	std::shared_ptr<MineBoard> mine_board = nullptr;	// ÆåÅÌ
	int x = 0;											// É¨À××ø±ê
	int y = 0;	
	bool is_sweep = true;								// Ö´ÐÐ²Ù×÷£ºÉ¨À×/±ê¼Ç
	bool debug_show = false;


public:
	virtual void on_enter() override
	{
		mine_board = std::shared_ptr<MineBoard>(new MineBoard(grid_x, grid_y, mine_count));
	}

	virtual void on_exit() override
	{
	}

	virtual void on_input(const SDL_Event& event) override
	{
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			// Êó±ê×ó¼üÉ¨À×£¬ÓÒ¼ü±ê¼Ç
			is_sweep = (event.button.button == SDL_BUTTON_LEFT);
			x = event.button.x;
			y = event.button.y;
			SDL_Log("x: %d, y: %d\n", x, y);
			break;
		case SDL_MOUSEBUTTONUP:
			if (is_sweep)
			{
				mine_board->sweep_point(x / grid_size, (y - status_h) / grid_size);
			}
			else
			{
				mine_board->flag_point(x / grid_size, (y - status_h) / grid_size);
			}
			SDL_Log("point idx, x: %d, y: %d\n", x / grid_size, (y - status_h) / grid_size);
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_q)
				debug_show = !debug_show;
			break;
		}
	}

	virtual void on_update(double delta) override
	{}

	virtual void on_render(SDL_Renderer* renderer) override
	{
		if (!debug_show)
			mine_board->on_render(renderer);
		else
			mine_board->show_mine(renderer);
	}
};