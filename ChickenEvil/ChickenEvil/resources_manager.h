#pragma once
#include "atlas.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <unordered_map>
#include <string>
#include <vector>

// 资源管理器单例
class ResourcesManager
{
private:
	static ResourcesManager* manager;
	std::unordered_map<std::string, SDL_Surface*> suf_image_pool;
	std::unordered_map<std::string, Atlas*> suf_atlas_pool;
	std::unordered_map<std::string, Mix_Music*> audio_pool;
	TTF_Font* font = nullptr;

private:
	ResourcesManager();
	~ResourcesManager();

public:
	static ResourcesManager* instance();
	void load();
	SDL_Surface* find_image(const std::string& id);
	Atlas* find_atlas(const std::string& id);
	Mix_Music* find_audio(const std::string& id);
	TTF_Font* get_font() const { return font; }
	//SDL_Texture* find_image_tex(const std::string& id);
	//SDL_Texture* find_atlas_tex(const std::string& id);
};