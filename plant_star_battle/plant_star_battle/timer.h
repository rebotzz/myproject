#pragma once
#include <functional>

// ͨ�ö�ʱ��
class Timer
{
private:
	int _pass_time = 0;					// �ѹ�ʱ��
	int _wait_time = 0;					// �ȴ�ʱ��
	bool _pause = false;				// �Ƿ���ͣ
	bool _shoted = false;				// �Ƿ񴥷�
	bool _one_shot = true;				// ���δ���
	std::function<void()> _callback;	// �����ص�

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