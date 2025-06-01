#pragma once

#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void set_wait_time(float second)
	{
		wait_time = second;
	}

	void set_one_shot(bool flag)
	{
		one_shot = flag;
	}

	void set_on_timeout(const std::function<void()>& callback)
	{
		on_time_out = callback;
	}

	void on_update(float delta)
	{
		if (is_pause) return;

		pass_time += delta;
		if (pass_time >= wait_time)
		{
			pass_time -= wait_time;
			bool can_shot = (!one_shot || (one_shot && !shotted));
			shotted = true;				// debug:放在on_time_out之前,避免回调逻辑内部设置重新启动
			if (can_shot && on_time_out)
				on_time_out();
		}
	}

	void restart()
	{
		pass_time = 0;
		is_pause = false;
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

private:
	float wait_time = 0;
	float pass_time = 0;
	bool is_pause = false;
	bool one_shot = true;
	bool shotted = false;
	std::function<void()> on_time_out;
};