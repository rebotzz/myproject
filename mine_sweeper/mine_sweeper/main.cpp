#include "scene_manager.h"
#include "game_scene.h"


// ɨ�׸�����Ŀ
int grid_x = 16;
int grid_y = 16;
int mine_count = 30;
const int grid_size = 40;
const int status_h = 100;
int window_w = 800;
int window_h = 800;
const int FPS = 15;				// todo: ������Բ�����FPS,ÿ�ε������»���/���ÿ�����һ��
TTF_Font* font = nullptr;

#undef main
int main()
{
	// ��ʼ��
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	window_w = grid_x * grid_size;
	window_h = grid_y * grid_size + status_h;
	SDL_Window* window = SDL_CreateWindow(u8"��ɨ�ס�By rebotzz.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_w, window_h, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_Event event;
	font = TTF_OpenFont("./res/font/msyh.ttc", 30);

	SceneManager::instance()->register_scene("game", new GameScene);
	SceneManager::instance()->switch_scene("game");

	Uint64 last_tick = SDL_GetPerformanceCounter();
	double tick_delta = 1.0 / SDL_GetPerformanceFrequency();
	double frame_delta = 1.0 / FPS;

	bool is_quit = false;
	while (!is_quit)
	{
		// ������Ϣ
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type)
			{
				is_quit = true;
				break;
			}
			SceneManager::instance()->on_input(event);
		}
		// ��������
		SceneManager::instance()->on_update(frame_delta);	// ������Ԥ��֡������
		// ��Ⱦ����
		SDL_RenderClear(renderer);
		SceneManager::instance()->on_render(renderer);
		SDL_RenderPresent(renderer);
		// ��̬��ʱ
		Uint64 cur_tick = SDL_GetPerformanceCounter();
		double delay = frame_delta - (cur_tick - last_tick) * tick_delta;
		last_tick = cur_tick;
		if (delay > 0.0001)
		{
			SDL_Delay((Uint32)(delay * 1000));
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	return 0;
}