#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>


const int WINDOW_W = 1280, WINDOW_H = 720;

#undef main
int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	

	SDL_Window* win =  SDL_CreateWindow(u8"Æ´ºÃ·¹´«Ææ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	SDL_Renderer* render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

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


	}


	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}