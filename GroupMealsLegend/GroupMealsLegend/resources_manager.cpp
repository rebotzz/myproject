#include "resources_manager.h"
#include <filesystem>
#include <fstream>

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
			else if (path.extension() == ".txt")
			{
				std::fstream file;
				file.open(path);
				std::vector<std::string>& text = script_pool[path.stem().u8string()];
				std::string str;

				while (getline(file, str))
				{
					text.emplace_back(std::move(str));
				}
				file.close();
			}
		}
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
	for (auto& iter : script_pool)
	{
		iter.second.clear();
	}
	texture_pool.clear();
	audio_pool.clear();
	font_pool.clear();
	script_pool.clear();
}
SDL_Texture* ResMgr::find_texture(const std::string& id)
{
	if (!texture_pool.count(id))
	{
#ifdef DEBUG
		SDL_Log("not find texture: %s\n", id.c_str());
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
		SDL_Log("not find texture: %s\n", id.c_str());
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
		SDL_Log("not find texture: %s\n", id.c_str());
#endif // DEBUG
		return nullptr;
	}
	else return font_pool[id];
}
const std::vector<std::string>& ResMgr::find_script(const std::string& id)
{
	if (!script_pool.count(id))
	{
#ifdef DEBUG
		SDL_Log("not find texture: %s\n", id.c_str());
#endif // DEBUG
		return {};
	}
	else return script_pool[id];
}
