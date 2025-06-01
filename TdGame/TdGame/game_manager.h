#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "singleton.h"
#include "resources_manager.h"
#include "config_manager.h"
#include "enemy_manager.h"
#include "collision_manager.h"
#include "bullet_manager.h"
#include "wave_manager.h"
#include "tower_manager.h"
#include "player_dragon_manager.h"
#include "panel.h"
#include "status_bar.h"

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <string>
#include <ctime>

// 操作：鼠标左键布置/升级塔防；右键拾取金币
// WASD控制闪电龙，JK闪电龙攻击

class GameMgr : public Singleton<GameMgr>
{
	friend Singleton<GameMgr>;
public:
	int run()
	{
		Uint64 last_tick = SDL_GetPerformanceCounter();
		Uint64 counter_freq = SDL_GetPerformanceFrequency();

		Mix_FadeInMusic(ResMgr::instance()->find_music(ResID::Mus_Bgm), -1, 1000);

		while (!is_quit)
		{
			// 消息处理
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					is_quit = true;
					break;
				}
				else
				{
					if (is_game_over) break;
					ui_panel.on_input(event);
					CoinMgr::instance()->on_input(event);
					PlayerDragonMgr::instance()->on_input(event);
				}
			}

			Uint64 cur_tick = SDL_GetPerformanceCounter();
			float delta = (cur_tick - last_tick) / (float)counter_freq;
			last_tick = cur_tick;
			if (delta < 1.0 / FPS)
			{
				SDL_Delay((Uint32)((1.0 / FPS - delta) * 1000.0));
				delta = 1.0 / FPS;
			}

			// 数据更新
			if (HomeMgr::instance()->get_hp() <= 0 || WaveMgr::instance()->is_wave_finished()) is_game_over = true;
			if (!is_game_over)
			{
				WaveMgr::instance()->on_update(delta);
				TowerMgr::instance()->on_update(delta);
				BulletMgr::instance()->on_update(delta);
				EnemyMgr::instance()->on_update(delta);
				CoinMgr::instance()->on_update(delta);
				PlayerDragonMgr::instance()->on_update(delta);
				CollisionMgr::instance()->process_collision();
			}

			// 渲染图片
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			render_map();
			EnemyMgr::instance()->on_renderer(renderer);
			TowerMgr::instance()->on_renderer(renderer);
			BulletMgr::instance()->on_renderer(renderer);
			HomeMgr::instance()->on_render(renderer);
			CoinMgr::instance()->on_render(renderer);
			PlayerDragonMgr::instance()->on_render(renderer);
			//CollisionMgr::instance()->on_debug_render(renderer);
			ui_panel.on_render(renderer);
			ui_status_bar.on_render(renderer);
			if (is_game_over) render_game_finished();

			SDL_RenderPresent(renderer);
		}

		return 0;
	}

protected:
	GameMgr()
	{
		// 初始化SDL组件
		init_assert(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO), "SDL_Init failed.");
		init_assert(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "IMG_Init failed.");
		init_assert(!Mix_Init(MIX_INIT_MP3), "Mix_Init failed.");
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
		init_assert(TTF_Init(), "TTF_Init failed.");
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");		// 输入法提示词相关?

		// 初始化窗口
		ConfigMgr* config = ConfigMgr::instance();
		window = SDL_CreateWindow(config->window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			config->window_width, config->window_height, SDL_WINDOW_SHOWN);
		init_assert(!window, "create window failed.");
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		init_assert(!renderer, "create renderer failed.");
		init_assert(!ResMgr::instance()->load(renderer, "resources"), "init resources failed.");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		//SDL_ShowCursor(SDL_FALSE);
		srand((unsigned int)time(nullptr));

		// 创建地图纹理
		create_map_texture();
	}
	~GameMgr()
	{
		// 释放资源
		SDL_DestroyTexture(texture_map);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		TTF_Quit();
		Mix_CloseAudio();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}

private:
	void init_assert(bool flag, const char* err_msg)
	{
		if (!flag) return;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"游戏初始化失败", err_msg, window);
		exit(-1);
	}

	// 预渲染地图,创建纹理图片
	void create_map_texture()
	{
		// 依据地图数据创建地图纹理
		TileMap& tile_map = ConfigMgr::instance()->tile_map;
		assert(!tile_map.empty());
		int map_height = TILE_SIZE * tile_map.size();
		int map_width = TILE_SIZE * tile_map[0].size();
		SDL_Texture* tex_tile_set = ResMgr::instance()->find_texture(ResID::Tex_Tileset);
		int tileset_w = 0, tileset_h = 0;
		SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &tileset_w, &tileset_h);
		int tileset_num_x = tileset_w / TILE_SIZE;
		int tileset_num_y = tileset_h / TILE_SIZE;

		// debug: 参数改为SDL_TEXTUREACCESS_TARGET才能被render修改
		SDL_DestroyTexture(texture_map);
		texture_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, map_width, map_height);
		SDL_SetTextureBlendMode(texture_map, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer, texture_map);

		SDL_Rect srcrect = { 0,0, TILE_SIZE, TILE_SIZE };
		SDL_Rect dstrect = { 0,0, TILE_SIZE, TILE_SIZE };
		for (int y = 0; y < tile_map.size(); ++y)
		{
			for (int x = 0; x < tile_map[y].size(); ++x)
			{
				Tile& tile = tile_map[y][x];
				dstrect.x = TILE_SIZE * x;
				dstrect.y = TILE_SIZE * y;

				// 底层瓦片
				srcrect.x = (tile.terrian % tileset_num_x) * TILE_SIZE;
				srcrect.y = (tile.terrian / tileset_num_x) * TILE_SIZE;
				SDL_RenderCopy(renderer, tex_tile_set, &srcrect, &dstrect);
				// 装饰
				srcrect.x = (tile.decoration % tileset_num_x) * TILE_SIZE;
				srcrect.y = (tile.decoration / tileset_num_x) * TILE_SIZE;
				SDL_RenderCopy(renderer, tex_tile_set, &srcrect, &dstrect);
			}
		}

		SDL_SetRenderTarget(renderer, nullptr);
	}

	void render_map()
	{
		static ConfigMgr* config = ConfigMgr::instance();
		static int map_w = config->tile_map[0].size() * TILE_SIZE;
		static int map_h = config->tile_map.size() * TILE_SIZE;
		static SDL_Rect dstrect = { 0,0, config->window_width, config->window_height };
		// todo坐标转化问题，x = map_w / 2 - dstrect.w / 2, y = map_h / 2 - dstrect.h / 2; ??
		//static SDL_Rect srcrect = { map_w / 2 - dstrect.w / 2, map_h / 2 - dstrect.h / 2, dstrect.w, dstrect.h };
		static SDL_Rect srcrect = { 0,0, dstrect.w, dstrect.h };
		SDL_RenderCopy(renderer, texture_map, &srcrect, &dstrect);
	}

	void render_game_finished()
	{
		int win_w = 0, win_h = 0;
		SDL_GetWindowSize(window, &win_w, &win_h);
		static SDL_Rect rect_bg = { win_w / 2 - 1282 / 2, win_h / 2 - 209 / 2 , 1282, 209 };
		static SDL_Rect rect_text = { win_w / 2 - 646 / 2, win_h / 2 - 215 / 2, 646, 215 };
		SDL_RenderCopy(renderer, ResMgr::instance()->find_texture(ResID::Tex_UiGameOverBar), nullptr, &rect_bg);
		if (HomeMgr::instance()->get_hp() > 0)
		{
			// 游戏胜利
			SDL_RenderCopy(renderer, ResMgr::instance()->find_texture(ResID::Tex_UiWinText), nullptr, &rect_text);
		}
		else
		{
			// 游戏失败
			SDL_RenderCopy(renderer, ResMgr::instance()->find_texture(ResID::Tex_UiLossText), nullptr, &rect_text);
		}
	}

private:
	// 基础组件
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event = { 0 };
	bool is_quit = false;
	const int FPS = 60;
	bool is_game_over = false;
	// 地图
	SDL_Texture* texture_map = nullptr;
	// UI界面
	Panel ui_panel;
	StatusBar ui_status_bar;
};


#endif // !_GAME_MANAGER_H_




