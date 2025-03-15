#pragma once
#include <functional>

// 通用定时器
class Timer
{
private:
	float pass_time = 0;					// 已过时间
	float wait_time = 0;					// 等待时间
	bool is_pause = false;					// 是否暂停
	bool shotted = false;					// 是否触发
	bool one_shot = true;					// 单次触发
	std::function<void()> on_timeout;		// 触发回调

public:
	Timer() = default;
	~Timer() = default;

	void restart()
	{
		pass_time = 0;
		shotted = false;
	}

	void pause()
	{
		is_pause = true;
	}

	void resume()
	{
		is_pause = false;
	}

	void set_wait_time(float interval)
	{
		wait_time = interval;
	}

	void set_on_timeout(const std::function<void()>& callback)
	{
		this->on_timeout = callback;
	}

	void set_one_shot(bool flag)
	{
		one_shot = flag;
	}

	void on_update(float delta)
	{
		if (is_pause)
			return;

		pass_time += delta;
		if (pass_time >= wait_time)
		{
			bool can_shot = !one_shot || (one_shot && !shotted);
			shotted = true;
			if (can_shot && on_timeout)
				on_timeout();
			pass_time -= wait_time;
		}
	}
};