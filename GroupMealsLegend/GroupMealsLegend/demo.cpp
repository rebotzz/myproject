#include "resources_manager.h"
#include "cursor_manager.h"
#include "region_manager.h"
#include "goods_bundle.h"
#include "takeout_box.h"
#include "delivery_driver.h"
#include "microwave_oven.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <ctime>

const int WINDOW_W = 1280, WINDOW_H = 720;
const int FPS = 60;
SDL_Window* win = nullptr;
SDL_Renderer* renderer = nullptr;



inline void render_background()
{
	static SDL_Texture* texture = ResMgr::instance()->find_texture("background");
	static SDL_Rect rect = { 0, 0 , 1280, 720 };
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

#undef main
int main()
{
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


	Mix_FadeInChannel(-1, ResMgr::instance()->find_audio("bgm"), -1, 500);

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
				RegionMgr::instance()->on_input(event);
				break;
			}
		}

		// 数据更新
		static int freq = SDL_GetPerformanceFrequency();
		static int cur_tick = 0, last_tick = SDL_GetPerformanceCounter();
		cur_tick = SDL_GetPerformanceCounter();
		double delta = (cur_tick - last_tick) / (double)freq;
		last_tick = cur_tick;
		RegionMgr::instance()->on_update(delta);

		// 渲染画面
		SDL_RenderClear(renderer);
		render_background();
		RegionMgr::instance()->on_render(renderer);
		//RegionMgr::instance()->on_debug_render(renderer);
		CursorMgr::instance()->on_render(renderer);
		SDL_RenderPresent(renderer);

		// 帧率控制
		int delay = 1000.0 / FPS - (delta + (SDL_GetPerformanceCounter() - last_tick) / (double)freq);
		if (delay > 0)
			SDL_Delay((int)(delay * 1000));	//ms
	}

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}