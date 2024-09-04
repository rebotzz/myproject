#pragma once
#include <vector>
#include <easyx.h>


class Atlas
{
private:
	std::vector<IMAGE> image_list;

public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_image, int num)
	{
		// º”‘ÿ∂Øª≠÷°Õº∆¨
		image_list.clear();
		image_list.resize(num);
		TCHAR file_path[512] = { 0 };
		for (int i = 0; i < num; ++i) {
			_stprintf_s(file_path, path_image, i + 1);
			loadimage(&image_list[i], file_path);
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

	IMAGE* get_image(int idx)
	{
		if (idx < 0 || idx >= image_list.size())
			return nullptr;

		return &image_list[idx];
	}

	void add_image(const IMAGE& img)
	{
		image_list.push_back(img);
	}

};