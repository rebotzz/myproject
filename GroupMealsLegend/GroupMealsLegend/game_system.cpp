#include "game_system.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "resources_manager.h"
#include "cursor_manager.h"
#include "region_manager.h"
#include "goods_bundle.h"
#include "takeout_box.h"
#include "delivery_driver.h"
#include "microwave_oven.h"

#include <ctime>

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
	Mix_FreeChunk(ResMgr::instance()->find_audio(bgm));
	Mix_FadeInChannel(-1, ResMgr::instance()->find_audio(id), -1, 500);
	bgm = id;
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


	// 输入
	// 更新
	// 渲染
}

