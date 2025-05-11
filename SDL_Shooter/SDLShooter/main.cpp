#define SDL_MAIN_HANDLED
#include <iostream>
#include "json/json.h"
#include "SDL.h"

void test_json()
{
    std::cout << "test\n";

    Json::Value root;
    root["name"] = "neo";
    root["id"] = 2398085;
    std::cout << root << std::endl;
}

void test_sdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow(u8"test window", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Point cursor_pos = {0};

    SDL_Event event;
    bool is_quit = false;
    while(!is_quit)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                is_quit = true;
                break;
                case SDL_MOUSEMOTION:
                cursor_pos.x = event.motion.x;
                cursor_pos.y = event.motion.y;
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);
        SDL_Rect rect = {cursor_pos.x, cursor_pos.y, 30, 30};
        SDL_SetRenderDrawColor(renderer, 0,255,255,255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);

        SDL_Delay(50);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char** argv)
{

    test_json();

    return 0;
}