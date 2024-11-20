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
	{"bottom",			"resources/bottom.png"},
	{"battery",			"resources/battery.png"},
	{"bullet",			"resources/bullet.png"},
	{"heart",			"resources/heart.png"},
	{"crosshair",		"resources/crosshair.png"},
	{"barrel_idle",		"resources/barrel_idle.png"},
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
// 长音频，只能单个音乐
static const std::vector<ResInfo> audioL_info_list =
{
	{"bgm",			"resources/bgm.mp3"},
	{"loss",		"resources/loss.mp3"},
};
// 短音频，可以播放多个，每个音乐会占用一个通道
static const std::vector<ResInfo> audioS_info_list = {
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

SDL_Surface* ResourcesManager::find_image(const std::string& id) const
{
	auto iter = suf_image_pool.find(id);
	if (iter == suf_image_pool.end())
		return nullptr;

	return iter->second;
}

Atlas* ResourcesManager::find_atlas(const std::string& id) const
{
	auto iter = suf_atlas_pool.find(id);
	if (iter == suf_atlas_pool.end())
		return nullptr;

	return iter->second;
}

Mix_Music* ResourcesManager::find_audio_music(const std::string& id) const
{
	auto iter = long_audio_pool.find(id);
	if (iter == long_audio_pool.end())
		return nullptr;

	return iter->second;
}

Mix_Chunk* ResourcesManager::find_audio_chunk(const std::string& id) const
{
	auto iter = short_audio_pool.find(id);
	if (iter == short_audio_pool.end())
		return nullptr;

	return iter->second;
}


TTF_Font* ResourcesManager::find_font(const std::string& id) const
{ 
	auto iter = font_pool.find(id);
	if (iter == font_pool.end())
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
	for (auto& info : audioL_info_list)
	{
		// 音乐的SDL_mixer数据类型为 Mix_Music，短音的数据类型为 Mix_Chunk。
		// 加载音乐（比较长的音乐），一般这个只有一个通道，所以使用这个不能同时播放音乐
		Mix_Music* music = Mix_LoadMUS(info.path.c_str());
		if (!music)
		{
			throw info.id;
		}
		long_audio_pool[info.id] = music;
	}
	for (auto& info : audioS_info_list)
	{
		// 如果需要同时播放音乐，可以使用下面的函数，一般电脑上通道限制可能是 8 个，每个音乐会占用一个通道
		// 音效（短音）一般用这个，虽然函数名后缀是 WAV，不过除了 wav 也能加载其他格式，如 mp3
		Mix_Chunk* music = Mix_LoadWAV(info.path.c_str());
		if (!music)
		{
			throw info.id;
		}
		short_audio_pool[info.id] = music;
	}

	// 加载导入字体
	TTF_Font* font = TTF_OpenFont("resources/IPix.ttf", 32);
	if (!font)
	{
		throw std::string("TTF_OpenFont failed, ") + std::string(TTF_GetError());
	}
	font_pool["IPix"] = font;
}

