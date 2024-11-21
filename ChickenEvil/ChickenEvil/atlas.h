#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <string>

// v0.2	SDL_Surface改为SDL_Texture

// 图集类	
class Atlas
{
private:
	std::vector<SDL_Texture*> image_list;

public:
	Atlas() = default;
	~Atlas() = default;

	void load(SDL_Renderer* renderer, const std::string& path_template, int num)
	{
		// 加载动画帧图片
		image_list.clear();
		image_list.resize(num);

		char buffer[512] = { 0 };
		for (int i = 0; i < num; ++i) {
			sprintf_s(buffer, path_template.c_str(), i + 1);
			image_list[i] = IMG_LoadTexture(renderer, buffer);		// IMG_Load改为IMG_LoadTexture
		}
	}

	void clear()
	{
		image_list.clear();
	}

	int get_size() const
	{
		return (int)image_list.size();
	}

	SDL_Texture* get_texture(int idx)
	{
		if (idx < 0 || idx >= image_list.size())
			return nullptr;

		return image_list[idx];
	}

	void add_texture(SDL_Texture *tex)
	{
		image_list.push_back(tex);
	}

};