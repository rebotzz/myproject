#pragma once
#include <functional>

// ͨ�ö�ʱ��
class Timer
{
private:
	float pass_time = 0;					// �ѹ�ʱ��
	float wait_time = 0;					// �ȴ�ʱ��
	bool is_pause = false;					// �Ƿ���ͣ
	bool shotted = false;					// �Ƿ񴥷�
	bool one_shot = true;					// ���δ���
	std::function<void()> on_timeout;		// �����ص�

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