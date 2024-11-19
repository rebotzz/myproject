#include "resources_manager.h"
#include "util.h"

struct ResInfo
{
	std::string id;
	std::string path;
};

struct AtlasResInfo
{
	std::string id;
	std::string path;
	int num_frame = 0;
};

static const std::vector<ResInfo> image_info_list =
{
	{"background",		"resources/background.png"},
	{"battery",			"resources/battery.png"},
	{"bullet",			"resources/bullet.png"},
	{"heart",			"resources/heart.png"},
	{"crosshair",		"resources/crosshair.png"},
};

static const std::vector<AtlasResInfo> atlas_info_list =
{
	{"barrel_fire",		"resources/barrel_fire_%d.png",  3},
	{"chicken_fast",	"resources/chicken_fast_%d.png",  4},
	{"chicken_medium",	"resources/chicken_medium_%d.png",  6},
	{"chicken_slow",	"resources/chicken_slow_%d.png",  8},
	{"explosion",		"resources/explosion_%d.png",  5},
};

// 音频资源
static const std::vector<ResInfo> audio_info_list =
{
	{"bgm",			"resources/bgm.mp3"},
	{"loss",		"resources/loss.mp3"},
	{"explosion",	"resources/explosion.wav"},
	{"fire_1",		"resources/fire_1.wav"},
	{"fire_2",		"resources/fire_2.wav"},
	{"fire_3",		"resources/fire_3.wav"},
	{"hurt",		"resources/hurt.wav"},
};

ResourcesManager* ResourcesManager::manager = nullptr;
ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager() = default;
ResourcesManager* ResourcesManager::instance()
{
	if (!manager)
		manager = new ResourcesManager();

	return manager;
}

SDL_Surface* ResourcesManager::find_image(const std::string& id)
{
	auto iter = suf_image_pool.find(id);
	if (iter == suf_image_pool.end())
		return nullptr;

	return iter->second;
}

Atlas* ResourcesManager::find_atlas(const std::string& id)
{
	auto iter = suf_atlas_pool.find(id);
	if (iter == suf_atlas_pool.end())
		return nullptr;

	return iter->second;
}

Mix_Music* ResourcesManager::find_audio(const std::string& id)
{
	auto iter = audio_pool.find(id);
	if (iter == audio_pool.end())
		return nullptr;

	return iter->second;
}

void ResourcesManager::load()
{
	// 加载图片
	for (auto& info : image_info_list)
	{
		SDL_Surface* suf_img =  IMG_Load(info.path.c_str());
		if (!suf_img)
			throw info.path;

		suf_image_pool[info.id] = suf_img;
	}

	for (auto& info : atlas_info_list)
	{
		Atlas* atlas = new Atlas();
		atlas->load(info.path, info.num_frame);

		for (int i = 0; i < atlas->get_size(); ++i)
		{
			if (!atlas->get_image(i))
				throw info.path;
		}

		suf_atlas_pool[info.id] = atlas;
	}

	// 加载音频
	for (auto& info : audio_info_list)
	{
		Mix_Music* music = Mix_LoadMUS(info.path.c_str());
		if (!music)
		{
			throw info.id;
		}
		audio_pool[info.id] = music;
	}

	// 加载导入字体
	font = TTF_OpenFont("resources/IPix.ttf", 32);
	if (!font)
	{
		throw std::string("TTF_OpenFont failed, ") + std::string(TTF_GetError());
	}
}

