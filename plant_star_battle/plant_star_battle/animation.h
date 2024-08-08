#pragma once
#include <functional>
#include "util.h"
#include "atlas.h"


class Animation
{
private:
	int _timer = 0;						// 计时器
	int _interval_ms = 0;				// 帧间隔
	int _idx_frame = 0;					// 帧索引
	Atlas* _atlas = nullptr;			// 图片集
	bool _is_loop = false;
	std::function<void()> _callback;	// 动画结束处理

public:
	Animation() = default;
	~Animation() = default;

	void reset()
	{
		_timer = 0;
		_idx_frame = 0;
	}

	void set_callback(const std::function<void()>& callback)
	{
		_callback = callback;
	}

	void set_atlas(Atlas* atlas)
	{
		reset();
		_atlas = atlas;
	}

	void set_interval(int ms)
	{
		_interval_ms = ms;
	}

	void set_loop(bool flag)
	{
		_is_loop = flag;
	}

	bool check_finished() const
	{
		if (_is_loop) return false;
		return _idx_frame == (_atlas->get_size() - 1);
	}

	int get_idx_frame() const
	{
		return _idx_frame;
	}

	IMAGE* get_frame() const
	{
		return _atlas->get_image(_idx_frame);
	}

	void on_update(int ms)
	{
		_timer += ms;
		if (_timer >= _interval_ms)
		{
			_timer = 0;
			_idx_frame++;
			if (_idx_frame >= _atlas->get_size())
			{
				_idx_frame = _is_loop ?  0 : _atlas->get_size() - 1;
				if (!_is_loop && _callback)	// function重载了operator bool,检测是否可调用
				{
					_callback();
				}
			}
		}
	} 

	void on_draw(int x, int y) const
	{
		putimage_alpha(x, y, _atlas->get_image(_idx_frame));
	}
};