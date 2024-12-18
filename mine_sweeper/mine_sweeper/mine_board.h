#pragma once
#include "util.h"
#include <vector>
#include <string>
#include <ctime>
#include <SDL.h>
#include <SDL_ttf.h>

extern const int grid_size;
extern const int status_h;
extern TTF_Font* font;

class MineBoard
{
private:
	enum
	{
		NONE = -1,								// 未探测区域标记
		FLAG = -2,								// 红旗标记
		MINE = -3								// 地雷标记
	};

private:
	std::vector<std::vector<int>> show_board;	// 展示棋盘
	std::vector<std::vector<bool>> mine_board;	// 地雷棋盘
	int grid_x, grid_y;							// 棋盘大小
	int mine_count;								// 地雷个数
	int flag_count;								// 标记个数
	int remain_grid;							// 未探测网格个数
	bool alive = true;							// 是否存活

public:
	MineBoard(int _grid_x, int _grid_y, int _mine_count)
		:grid_x(_grid_x), grid_y(_grid_y), mine_count(_mine_count), flag_count(0), remain_grid(_grid_x * _grid_y)
	{
		show_board.resize(grid_x, std::vector<int>(grid_y, NONE));
		mine_board.resize(grid_x, std::vector<bool>(grid_y, false));
		srand((unsigned int)time(nullptr));

		if (mine_count >= grid_x * grid_y)
		{
			for (int i = 0; i < grid_x; ++i)
			{
				for (int j = 0; j < grid_y; ++j)
				{
					mine_board[i][j] = true;
				}
			}
			return;
		}
		for (int i = 0; i < mine_count; ++i)
		{
			bool valid = false;
			while (!valid)
			{
				int x = rand() % grid_x;
				int y = rand() % grid_y;
				if (!mine_board[x][y])
				{
					mine_board[x][y] = true;
					valid = true;
				}
			}
		}
	}

	void sweep_point(int x, int y)
	{
		if (x < 0 || x >= grid_x || y < 0 || y >= grid_y)
			return;

		// dfs
		if (mine_board[x][y])
		{
			show_board[x][y] = MINE;
			alive = false;
		}
		else
		{
			dfs(x, y);
		}
	}

	void flag_point(int x, int y)
	{
		if (x < 0 || x >= grid_x || y < 0 || y >= grid_y)
			return;

		if (NONE == show_board[x][y])
		{
			++flag_count;
			show_board[x][y] = FLAG;
		}
		else if (FLAG == show_board[x][y])
		{
			--flag_count;
			show_board[x][y] = NONE;
		}
	}

	int get_mine_count() const
	{
		return mine_count - flag_count;
	}

	bool is_alive() const
	{
		return alive;
	}

	bool is_seccess() const
	{
		return mine_count == remain_grid;
	}

	void on_render(SDL_Renderer* renderer)
	{
		// 方块
		static SDL_Rect rect_grid = { 0, 0, grid_size, grid_size };
		static SDL_Rect rect_flag = { 0, 0, grid_size * 0.5, grid_size * 0.5 };
		static SDL_Rect rect_text = { 0, 0, grid_size, grid_size };

		for (int i = 0; i < grid_x; ++i)
		{
			for (int j = 0; j < grid_y; ++j)
			{
				// 底色
				switch (show_board[i][j])
				{
				case NONE:
				case FLAG:
				case MINE:
					SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
					rect_grid.x = i * grid_size;
					rect_grid.y = j * grid_size + status_h;
					SDL_RenderFillRect(renderer, &rect_grid);
					break;
				default:
					SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);
					rect_grid.x = i * grid_size;
					rect_grid.y = j * grid_size + status_h;
					SDL_RenderFillRect(renderer, &rect_grid);
					break;
				}
				// 标记
				switch (show_board[i][j])
				{
				case FLAG:
					draw_flag(renderer, i * grid_size, j * grid_size + status_h, grid_size);
					break;
				case MINE:
					SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
					rect_flag.x = i * grid_size + (rect_grid.w - rect_flag.w) / 2;
					rect_flag.y = j * grid_size + status_h + (rect_grid.h - rect_flag.h) / 2;
					SDL_RenderFillRect(renderer, &rect_flag);
					break;
				default:
					if (show_board[i][j] > 0)
					{
						SDL_Color color = { 255,255,255,255 };
						switch (show_board[i][j])
						{
						case 1:
							color = { 0,0,255,255 };
							break;
						case 2:
							color = { 0,140,0,255 };
							break;
						case 3:
							color = { 255,0,0,255 };
							break;
						case 4:
							color = { 65,105,255,255 };
							break;
						default:
							color = { 132,112,255,255 };
							break;
						}

						SDL_Surface* suf_text = TTF_RenderText_Blended(font, std::to_string(show_board[i][j]).c_str(), color);
						SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
						rect_text.x = i * grid_size + (grid_size - suf_text->w) / 2;
						rect_text.y = j * grid_size + status_h + (grid_size - suf_text->h) / 2;
						rect_text.w = suf_text->w;
						rect_text.h = suf_text->h;
						SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
						//SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
						//SDL_RenderFillRect(renderer, &rect_grid);
						SDL_DestroyTexture(tex_text);
						SDL_FreeSurface(suf_text);
					}
					break;
				}
			}
		}

		// 网格
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		for (int i = 0; i < grid_x; ++i)
		{
			SDL_RenderDrawLine(renderer, i * grid_size, status_h, i * grid_size, status_h + grid_y * grid_size);
		}
		for (int i = 0; i < grid_y; ++i)
		{
			SDL_RenderDrawLine(renderer, 0, status_h + i * grid_size, grid_x * grid_size, status_h + i * grid_size);
		}
	}

	void show_mine(SDL_Renderer* renderer)
	{
		// 方块
		static SDL_Rect rect = { 0, status_h, grid_x * grid_size, status_h + grid_y * grid_size };
		static SDL_Rect rect_flag = { 0, 0, grid_size * 0.5, grid_size * 0.5 };

		SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
		SDL_RenderFillRect(renderer, &rect);
		for (int i = 0; i < grid_x; ++i)
		{
			for (int j = 0; j < grid_y; ++j)
			{
				if (mine_board[i][j])
				{
					SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
					rect_flag.x = i * grid_size + (grid_size - rect_flag.w) / 2;
					rect_flag.y = j * grid_size + status_h + (grid_size - rect_flag.h) / 2;
					SDL_RenderFillRect(renderer, &rect_flag);
				}
			}
		}

		// 网格
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		for (int i = 0; i < grid_x; ++i)
		{
			SDL_RenderDrawLine(renderer, i * grid_size, status_h, i * grid_size, status_h + grid_y * grid_size);
		}
		for (int i = 0; i < grid_y; ++i)
		{
			SDL_RenderDrawLine(renderer, 0, status_h + i * grid_size, grid_x * grid_size, status_h + i * grid_size);
		}
	}

private:
	void dfs(int x, int y)
	{
		if (show_board[x][y] != NONE)
			return;

		--remain_grid;
		int side_mine = get_side_mine(x, y);
		show_board[x][y] = side_mine;
		if (side_mine == 0)
		{
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					if (0 == i && 0 == j) continue;
					int x_ = x + i, y_ = y + j;
					if (x_ >= 0 && x_ < grid_x && y_ >= 0 && y_ < grid_y)
					{
						dfs(x_, y_);
					}
				}
			}
		}
	}

	int get_side_mine(int x, int y) const
	{
		int side_mine = 0;
		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; ++j)
			{
				int x_ = x + i, y_ = y + j;
				if (x_ >= 0 && x_ < grid_x && y_ >= 0 && y_ < grid_y && mine_board[x_][y_])
				{
					side_mine++;
				}
			}
		}
		return side_mine;
	}


};