#include "resources_manager.h"
#include "character_manager.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <ctime>
#include <Windows.h>

#undef main

const int WINDOW_W = 1280;
const int WINDOW_H = 720;
const int FPS = 60;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int main()
{
	// 初始化SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_MP3);	// sdl核心库有.wav
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);	// MIX_DEFAULT_FREQUENCY 44100

	window = SDL_CreateWindow(u8"生化危鸡", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	srand(time(nullptr));

	// 资源加载
	try
	{
		ResourcesManager::instance()->load();
	}
	catch (const std::string s)
	{
		SDL_Log("ResourcesManager error, %s", s.c_str());
		system("pause");
		exit(-1);
	}

	// 敌人，机枪,玩家生命，摄像机
	Camera scene_camera, ui_camera;



	// 时间
	Uint64 freq = SDL_GetPerformanceFrequency();
	Uint64 last_tick, cur_tick;
	last_tick = SDL_GetPerformanceCounter();
	SDL_Texture* tex_img_bg = SDL_CreateTextureFromSurface(renderer, ResourcesManager::instance()->find_image("background"));


	Mix_FadeInMusic(ResourcesManager::instance()->find_audio_music("bgm"), -1, 10);

	// 主循环
	SDL_Event event;
	bool is_quit = false;
	while (!is_quit)
	{
		// 读取消息
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				is_quit = true;
				break;
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				CharacterManager::instance()->on_input(&event);
				break;
			}
		}

		// 更新数据
		cur_tick = SDL_GetPerformanceCounter();
		double delta = (cur_tick - last_tick) / (double)freq;
		last_tick = cur_tick;
		CharacterManager::instance()->on_update(delta);
		CollisionManager::instance()->process_collide();


		// 渲染画面
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_Rect rect_bg = { (int)scene_camera.get_position().x, (int)scene_camera.get_position().y, WINDOW_W, WINDOW_H };
		SDL_RenderCopy(renderer, tex_img_bg, nullptr, &rect_bg);
		CharacterManager::instance()->on_render(renderer, scene_camera);
		CharacterManager::instance()->render_status(renderer, ui_camera);
		//CollisionManager::instance()->on_debug_render(renderer, scene_camera);

		SDL_RenderPresent(renderer);

		// 动态延时
		double delay = delta + (SDL_GetPerformanceCounter() - last_tick) / (double)freq - 1.0 / (double)FPS;
		if (delay > 0)
			SDL_Delay(delay);
	}

	SDL_DestroyTexture(tex_img_bg);

	// 关闭SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

	return 0;
}


void render_background(SDL_Texture* tex_img_bg)
{

}