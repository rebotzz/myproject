#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <string>
#include <unordered_map>
#include <vector>

class ResMgr
{
private:
	static ResMgr* manager;
	ResMgr() = default;
	~ResMgr() = default;

private:
	std::unordered_map<std::string, SDL_Texture*> texture_pool;				// 图片纹理
	std::unordered_map<std::string, Mix_Chunk*> audio_pool;					// 音频资源
	std::unordered_map<std::string, TTF_Font*> font_pool;					// 字体格式
	std::unordered_map<std::string, std::vector<std::string>> script_pool;	// 对话文本

public:

	static ResMgr* instance();
	void load(SDL_Renderer* renderer);
	void unload();
	SDL_Texture* find_texture(const std::string& id);
	Mix_Chunk* find_audio(const std::string& id);
	TTF_Font* find_font(const std::string& id);
	const std::vector<std::string>& find_script(const std::string& id);

};