#include "resources_manager.h"
#include <filesystem>

ResMgr* ResMgr::manager = nullptr;

ResMgr* ResMgr::instance()
{
	if (manager == nullptr)
	{
		manager = new ResMgr();
	}
	return manager;
}

void ResMgr::load(SDL_Renderer* renderer)
{
	using namespace std::filesystem;
	if (!exists(path("resources"))) return;
	for (auto& entry : directory_iterator("resources"))
	{
		const path& path = entry.path();
		if (is_regular_file(path))
		{
			if (path.extension() == ".png")
			{
				SDL_Texture* tex = IMG_LoadTexture(renderer, path.u8string().c_str());
				texture_pool[path.stem().u8string().c_str()] = tex;
			}
			else if (path.extension() == ".mp3")
			{
				Mix_Chunk* chunk = Mix_LoadWAV(path.u8string().c_str());
				audio_pool[path.stem().u8string().c_str()] = chunk;
			}
			else if (path.extension() == ".ttf")
			{
				TTF_Font* font = TTF_OpenFont(path.u8string().c_str(), 32);
				font_pool[path.stem().u8string().c_str()] = font;
			}
		}
		//SDL_Log("%s\n", path.u8string().c_str());
	}

}

void ResMgr::unload()
{
	for (auto& iter : texture_pool)
	{
		SDL_DestroyTexture(iter.second);
		texture_pool.erase(iter.first);
	}
	for (auto& iter : audio_pool)
	{
		Mix_FreeChunk(iter.second);
		audio_pool.erase(iter.first);
	}
	for (auto& iter : font_pool)
	{
		TTF_CloseFont(iter.second);
		font_pool.erase(iter.first);
	}
}
SDL_Texture* ResMgr::find_texture(const std::string& id)
{
	if (!texture_pool.count(id))
	{
#ifdef DEBUG
		throw std::string("not find resource: ") + id;
#endif // DEBUG

		return nullptr;
	}
	else return texture_pool[id];
}
Mix_Chunk* ResMgr::find_audio(const std::string& id)
{
	if (!audio_pool.count(id))
	{
#ifdef DEBUG
		throw std::string("not find resource: ") + id;
#endif // DEBUG
		return nullptr;
	}
	else return audio_pool[id];
}
TTF_Font* ResMgr::find_font(const std::string& id)
{
	if (!font_pool.count(id))
	{
#ifdef DEBUG
		throw std::string("not find resource: ") + id;
#endif // DEBUG
		return nullptr;
	}
	else return font_pool[id];
}
