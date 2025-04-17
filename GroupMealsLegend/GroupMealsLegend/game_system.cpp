#include "game_system.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "resources_manager.h"
#include "cursor_manager.h"
#include "region_manager.h"
#include "scene_manager.h"
#include "dialogue_manager.h"
#include <ctime>
#include <fstream>


GameSystem* GameSystem::manager = nullptr;
GameSystem* GameSystem::instance()
{
	if (!manager)
	{
		manager = new GameSystem();
	}
	return manager;
}

void GameSystem::set_mode(Mode val)
{
	mode = val;
}
GameSystem::Mode GameSystem::get_mode() const
{
	return mode;
}
void GameSystem::switch_bgm(const std::string& id)
{
	if(bgm_channel != -1)
		Mix_FadeOutChannel(bgm_channel, 500);
	bgm_channel = Mix_FadeInChannel(-1, ResMgr::instance()->find_audio(id), -1, 500);
	bgm = id;
}
void GameSystem::play_audio(const std::string& id)
{
	Mix_PlayChannel(-1, ResMgr::instance()->find_audio(id), 0);
}
void GameSystem::start()
{
	// 游戏初始化
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);

	win = SDL_CreateWindow(u8"《拼好饭传奇》-by rebotzz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_ShowCursor(SDL_DISABLE);
	Mix_AllocateChannels(12);
	srand((unsigned int)time(nullptr));

	// 初始化：加载资源
	ResMgr::instance()->load(renderer);
	DialogMgr::instance()->set_script_id("script_1");
	set_mode(Mode::Dialogue);
	SceneMgr::instance()->switch_scene("transition");
	load_game();

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		// 消息处理
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				CursorMgr::instance()->on_input(event);
				SceneMgr::instance()->on_input(event);
				DialogMgr::instance()->on_input(event);
				break;
			}
		}

		// 数据更新
		static int freq = SDL_GetPerformanceFrequency();
		static int cur_tick = 0, last_tick = SDL_GetPerformanceCounter();
		cur_tick = SDL_GetPerformanceCounter();
		double delta = (cur_tick - last_tick) / (double)freq;
		last_tick = cur_tick;
		SceneMgr::instance()->on_update(delta);
		DialogMgr::instance()->on_update(delta);

		// 渲染画面
		SDL_RenderClear(renderer);
		SceneMgr::instance()->on_render(renderer);
		DialogMgr::instance()->on_render(renderer);
		//RegionMgr::instance()->on_debug_render(renderer);
		CursorMgr::instance()->on_render(renderer);
		SDL_RenderPresent(renderer);

		// 帧率控制
		int delay = 1000.0 / FPS - (delta + (SDL_GetPerformanceCounter() - last_tick) / (double)freq);
		if (delay > 0)
			SDL_Delay((int)(delay * 1000));	//ms
	}

	save_game();

	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void GameSystem::set_coins_goal(int val)
{
	CursorMgr::instance()->set_coins_goal(val);
}
void GameSystem::set_drink_goal(const std::string& target)
{
	CursorMgr::instance()->set_drink_goal(target);
}
void GameSystem::finish_goal()
{
	DialogMgr::instance()->finish_goal();
	set_mode(Mode::Dialogue);
}
void GameSystem::save_game()
{
	// 存档格式：执行脚本id,执行到的位置，当前场景id，当前bgm，硬币数量
	std::ofstream file("save.txt", std::ios::out | std::ios::trunc);
	file << DialogMgr::instance()->get_script_id() << "\n";
	file << DialogMgr::instance()->get_idx() << "\n";
	file << SceneMgr::instance()->get_cur_scene_name() << "\n";
	file << bgm << "\n";
	file << CursorMgr::instance()->get_coins() << "\n";
	file.close();
}
void GameSystem::load_game()
{
	std::ifstream file("save.txt");
	if (!file.fail())
	{
		std::string script_id, scene_id, bgm_id;
		int script_line_idx = 0, coins = -1;
		file >> script_id;
		file >> script_line_idx;
		file >> scene_id;
		file >> bgm_id;
		file >> coins;
		if (coins < 0)
		{
			// 存档错误
			file.close();
			std::ofstream out("save.txt", std::ios::trunc);
			out.close();
			return;
		}
		DialogMgr::instance()->set_script_id(script_id);
		DialogMgr::instance()->set_idx(script_line_idx);
		SceneMgr::instance()->switch_scene(scene_id);
		switch_bgm(bgm_id);
		CursorMgr::instance()->add_coins(coins);
		file.close();
	}
}