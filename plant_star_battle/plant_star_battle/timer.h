#pragma once
#include <functional>

// 通用定时器
class Timer
{
private:
	int _pass_time = 0;					// 已过时间
	int _wait_time = 0;					// 等待时间
	bool _pause = false;				// 是否暂停
	bool _shoted = false;				// 是否触发
	bool _one_shot = true;				// 单次触发
	std::function<void()> _callback;	// 触发回调

public:
	Timer() = default;
	~Timer() = default;

	void restart()
	{
		_pass_time = 0;
		_shoted = false;
	}

	void pause()
	{
		_pause = true;
	}

	void resume()
	{
		_pause = false;
	}

	void set_wait_time(int ms)
	{
		_wait_time = ms;
	}

	void set_callback(const std::function<void()>& callback)
	{
		_callback = callback;
	}

	void set_one_shot(bool flag)
	{
		_one_shot = flag;
	}


	void on_update(int interval_ms)
	{
		if (_pause)
			return;
			
		_pass_time += interval_ms;
		if (_pass_time >= _wait_time)
		{
			if ((!_one_shot || (_one_shot && !_shoted)) && _callback)
				_callback();
			_pass_time = 0;
			_shoted = true;
		}
	}

};