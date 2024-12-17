#pragma comment (linker,"/subsystem:windows /entry:mainCRTStartup")		// �رտ���̨����
#define SDL_MAIN_HANDLED
#include "resources_manager.h"
#include "character_manager.h"
#include <SDL_main.h>

const int WINDOW_W = 1280;
const int WINDOW_H = 720;
const int FPS = 60;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
void render_background(const Camera& camera);

//#undef main
int main(int argc, char* argv[])
{
	// ��ʼ��SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_MP3);	// sdl���Ŀ���.wav
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);	// MIX_DEFAULT_FREQUENCY 44100

	window = SDL_CreateWindow(u8"������Σ���� By rebotzz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);	// SDL_RENDERER_SOFTWARE SDL_RENDERER_ACCELERATED
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_ShowCursor(false);		// ���������
	srand((unsigned int)time(nullptr));

	// ��Դ����
	try
	{
		ResourcesManager::instance()->load(renderer);
	}
	catch (const std::string& s)
	{
		SDL_Log("ResourcesManager error, %s", s.c_str());
		system("pause");
		exit(-1);
	}

	// ���ˣ���ǹ,��������������
	Camera scene_camera, ui_camera;

	// ʱ��
	Uint64 freq = SDL_GetPerformanceFrequency();
	Uint64 last_tick, cur_tick;
	last_tick = SDL_GetPerformanceCounter();

	// BGM
	Mix_FadeInMusic(ResourcesManager::instance()->find_audio_music("bgm"), -1, 100);

	// ��ѭ��
	SDL_Event event;
	bool is_quit = false;
	while (!is_quit)
	{
		// ��ȡ��Ϣ
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
				if (CharacterManager::instance()->is_alive())
				{
					if (event.type == SDL_MOUSEBUTTONDOWN) scene_camera.set_shake(true, 6);
					else if (event.type == SDL_MOUSEBUTTONUP) scene_camera.set_shake(false);
				}
				else
				{
					scene_camera.set_shake(false);
				}
				break;
			}
		}

		// ��������
		cur_tick = SDL_GetPerformanceCounter();
		double delta = (cur_tick - last_tick) / (double)freq;
		last_tick = cur_tick;
		CharacterManager::instance()->on_update((float)delta);
		CollisionManager::instance()->process_collide();
		scene_camera.on_update((float)delta);

		// ��Ⱦ����
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		render_background(scene_camera);
		CharacterManager::instance()->on_render(renderer, scene_camera);
		CharacterManager::instance()->render_status(renderer, ui_camera);
		//CollisionManager::instance()->on_debug_render(renderer, scene_camera);
		SDL_RenderPresent(renderer);

		// ��̬��ʱ
		double delay = 1.0 / (double)FPS - (delta + (SDL_GetPerformanceCounter() - last_tick) / (double)freq);
		if (delay > 0)
		{
			SDL_Delay((int)(delay * 1000.0));	// debug: ��λת��ms
		}
	}

	// �ر�SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

	return 0;
}


inline void render_background(const Camera& camera)
{
	static SDL_Texture* tex_bg = ResourcesManager::instance()->find_image("background");
	static SDL_Rect rect_bg = { 0, 0,  WINDOW_W, WINDOW_H };
	int bg_h = 0, bg_w = 0;
	SDL_QueryTexture(tex_bg, nullptr, nullptr, &bg_w, &bg_h);
	rect_bg.x = (bg_w - WINDOW_W) - (int)camera.get_position().x;
	rect_bg.y = (bg_h - WINDOW_H) - (int)camera.get_position().y;

	SDL_RenderCopy(renderer, tex_bg, &rect_bg, nullptr);
}

//// ����
//inline void game_over()
//{
//	// ����ж������ʱ���ƺ��¼������رգ�û��Ӧ
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
//
//	Mix_PlayMusic(ResourcesManager::instance()->find_audio_music("loss"), 0);
//	SDL_Window* end_window = nullptr;
//	SDL_Surface* suf_window = nullptr;
//	end_window = SDL_CreateWindow(u8"��Ϸ����!", SDL_WINDOWPOS_CENTERED,
//		SDL_WINDOWPOS_CENTERED, 300, 250, SDL_WINDOW_SHOWN);
//	suf_window = SDL_GetWindowSurface(end_window);
//
//	std::string str = "Score: " + std::to_string(CharacterManager::instance()->get_score());
//	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(ResourcesManager::instance()->find_font("IPix"),
//		str.c_str(), { 255, 255, 255, 255 });
//	SDL_Rect rect_text = { 50, 50, suf_text->w, suf_text->h };
//	SDL_BlitSurface(suf_text, nullptr, suf_window, &rect_text);
//	SDL_UpdateWindowSurface(end_window);
//
//	// ��ȡ��Ϣ
//	SDL_Event event;
//	while (true)
//	{
//		while (SDL_PollEvent(&event))
//		{
//			switch (event.type)
//			{
//			case SDL_QUIT:
//
//				SDL_DestroyWindow(end_window);
//				TTF_Quit();
//				IMG_Quit();
//				Mix_Quit();
//				SDL_Quit();
//				exit(0);
//				break;
//			}
//			SDL_Delay(1.0 / (double)FPS);
//		}
//	}
//	exit(0);
//}