#pragma once
#include <vector>
#include <cassert>
#include <easyx.h>

class Atlas
{
private:
	std::vector<IMAGE> _img_list;

public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_image, int num)
	{
		// º”‘ÿ∂Øª≠÷°Õº∆¨
		_img_list.clear();
		_img_list.resize(num);
		TCHAR buff[256] = { 0 };
		for (int i = 0; i < num; ++i) {
			_stprintf_s(buff, path_image, i + 1);
			loadimage(&_img_list[i], buff);
		}
	}

	void clear()
	{
		_img_list.clear();
	}

	int get_size()
	{
		return (int)_img_list.size();
	}

	IMAGE* get_image(int idx)
	{
		assert(_img_list.size() != 0);
		if (idx < 0 || idx >= _img_list.size())
			return nullptr;

		return &_img_list[idx];
	}

	void add_image(const IMAGE& img)
	{
		_img_list.push_back(img);
	}

};