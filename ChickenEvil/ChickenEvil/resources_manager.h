#pragma once
#include "atlas.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <unordered_map>
#include <string>
#include <vector>

// ��Դ����������
class ResourcesManager
{
private:
	static ResourcesManager* manager;
	std::unordered_map<std::string, SDL_Surface*> suf_image_pool;
	std::unordered_map<std::string, Atlas*> suf_atlas_pool;
	std::unordered_map<std::string, Mix_Music*> long_audio_pool;
	std::unordered_map<std::string, Mix_Chunk*> short_audio_pool;
	std::unordered_map<std::string, TTF_Font*> font_pool;

private:
	ResourcesManager();
	~ResourcesManager();

public:
	static ResourcesManager* instance();
	void load();
	SDL_Surface* find_image(const std::string& id) const;
	Atlas* find_atlas(const std::string& id) const;
	Mix_Music* find_audio_music(const std::string& id) const;
	Mix_Chunk* find_audio_chunk(const std::string& id) const;
	TTF_Font* find_font(const std::string& id) const;

	// TODO���ܣ����μ���ĳЩ��Դ���ϣ�Ȼ�������ͷţ�������Ҫ���ļ���ϣ�Ūһ��json֮�����Դ�嵥
	//void free(const std::string& kind, const std::string* id) {}	
};