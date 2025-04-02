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
	std::unordered_map<std::string, SDL_Texture*> texture_pool;				// ͼƬ����
	std::unordered_map<std::string, Mix_Chunk*> audio_pool;					// ��Ƶ��Դ
	std::unordered_map<std::string, TTF_Font*> font_pool;					// �����ʽ
	std::unordered_map<std::string, std::vector<std::string>> script_pool;	// �Ի��ı�

public:

	static ResMgr* instance();
	void load(SDL_Renderer* renderer);
	void unload();
	SDL_Texture* find_texture(const std::string& id);
	Mix_Chunk* find_audio(const std::string& id);
	TTF_Font* find_font(const std::string& id);
	const std::vector<std::string>& find_script(const std::string& id);

};