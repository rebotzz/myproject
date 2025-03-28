#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>
#include "resources_manager.h"

const int WINDOW_W = 1280, WINDOW_H = 720;
SDL_Window* win = nullptr;
SDL_Renderer* render = nullptr;

void test()
{
	ResMgr::instance()->load(render);
}


#undef main
int main()
{



	SDL_Init(SDL_INIT_EVERYTHING);
	

	win =  SDL_CreateWindow(u8"Æ´ºÃ·¹´«Ææ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);


	test();

	SDL_Event event;
	while (1)
	{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				return 0;
				break;
			}
		}

		

		SDL_Delay(100);
	}

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}