#pragma once
#include <functional>

class Timer
{
private:
	float wait_time = 0;
	float pass_time = 0;
	bool one_shot = true;
	bool shotted = false;
	bool is_pause = false;
	std::function<void(void)> on_timeout;

public:
	Timer() = default;
	~Timer() = default;

	void set_one_shot(bool val)
	{
		one_shot = val;
	}

	void set_wait_time(float val)
	{
		wait_time = val;
	}

	void set_on_timeout(const std::function<void(void)>& callback)
	{
		on_timeout = callback;
	}

	void on_update(float delta)
	{
		if (is_pause) return;
		pass_time += delta;
		if (pass_time >= wait_time)
		{
			bool can_shot = (one_shot && !shotted) || !one_shot;
			shotted = true;
			if (can_shot && on_timeout)
				on_timeout();
			pass_time -= wait_time;
		}
	}

	void pause()
	{
		is_pause = true;
	}

	void resume()
	{
		is_pause = false;
	}

	void restart()
	{
		pass_time = 0;
		is_pause = false;
		shotted = false;
	}
};
