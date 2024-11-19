#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <string>

// 图集类
class Atlas
{
private:
	std::vector<SDL_Surface*> image_list;

public:
	Atlas() = default;
	~Atlas() = default;

	void load(const std::string& path_image, int num)
	{
		// 加载动画帧图片
		image_list.clear();
		image_list.resize(num);

		char buffer[512] = { 0 };
		for (int i = 0; i < num; ++i) {
			sprintf_s(buffer, path_image.c_str(), i + 1);
			image_list[i] = IMG_Load(buffer);
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

	SDL_Surface* get_image(int idx)
	{
		if (idx < 0 || idx >= image_list.size())
			return nullptr;

		return image_list[idx];
	}

	void add_image(SDL_Surface *img)
	{
		image_list.push_back(img);
	}

};