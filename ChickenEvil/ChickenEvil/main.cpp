//#pragma comment (linker,"/subsystem:windows /entry:mainCRTStartup")		// 关闭控制台窗口

#define SDL_MAIN_HANDLED
#include "resources_manager.h"
#include "character_manager.h"
#include <SDL_main.h>

const int WINDOW_W = 1280;
const int WINDOW_H = 720;
const int FPS = 60;
float win_scale = 1.0f;				// 画面缩放比例
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Camera* scene_camera = nullptr, *ui_camera = nullptr;

void render_background(const Camera& camera);
void init();
void deinit();
void main_loop();

//#undef main
int main(int argc, char* argv[])
{
	init();
	main_loop();
	deinit();

	return 0;
}


void init()
{
	// 初始化SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_MP3);	// sdl核心库有.wav
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);	// MIX_DEFAULT_FREQUENCY 44100

	window = SDL_CreateWindow(u8"《生化危鸡》 by rebotzz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);	// SDL_RENDERER_SOFTWARE SDL_RENDERER_ACCELERATED
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_ShowCursor(false);		// 隐藏鼠标光标 SDL_DISABLE 0
	Mix_AllocateChannels(32);
	srand((unsigned int)time(nullptr));

	// 资源加载
	try
	{
		ResourcesManager::instance()->load(renderer);
	}
	catch (const std::string& s)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Resources Not Find", s.c_str(), window);
		exit(-1);
	}

	// BGM
	Mix_FadeInMusic(ResourcesManager::instance()->find_audio_music("bgm"), -1, 100);

	// 摄像机
	scene_camera = new Camera(renderer);
	ui_camera = new Camera(renderer);
}

void deinit()
{
	ResourcesManager::instance()->unload();
	if (scene_camera) delete scene_camera;
	if (ui_camera) delete ui_camera;

	// 关闭SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

void main_loop()
{
	// 时间
	Uint64 freq = SDL_GetPerformanceFrequency();
	Uint64 last_tick, cur_tick;
	last_tick = SDL_GetPerformanceCounter();

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
		CharacterManager::instance()->on_update((float)delta);
		if (!CharacterManager::instance()->is_alive())
		{
			Mix_HaltMusic();
			Mix_PlayMusic(ResourcesManager::instance()->find_audio_music("loss"), 0);
			std::string str = "Score: " + std::to_string(CharacterManager::instance()->get_score());
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, u8"游戏结束!", str.c_str(), window);
			is_quit = true;
		}
		CollisionManager::instance()->process_collide();
		scene_camera->on_update((float)delta);

		// 渲染画面
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		render_background(*scene_camera);
		CharacterManager::instance()->on_render(*scene_camera);
		CharacterManager::instance()->render_status(*ui_camera);
		//CollisionManager::instance()->on_debug_render(renderer, scene_camera);
		SDL_RenderPresent(renderer);

		// 动态延时
		double delay = 1.0 / (double)FPS - (delta + (SDL_GetPerformanceCounter() - last_tick) / (double)freq);
		if (delay > 0)
		{
			SDL_Delay((int)(delay * 1000.0));	// debug: 单位转化ms
		}
	}
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

