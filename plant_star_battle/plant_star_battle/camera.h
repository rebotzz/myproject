#pragma once
#include "vector2.h"
#include "timer.h"
#include <ctime>

class Camera
{
private:
	Vector2 _position;				// 摄像机位置
	Timer _timer_shake;				// 摄像机抖动计时器
	float _shaking_strength = 0;	// 摄像机抖动幅度
	bool _is_shaking = false;		// 摄像机是否抖动

public:
	Camera()
	{
		srand((unsigned int)time(nullptr));
		_timer_shake.set_one_shot(true);
		_timer_shake.set_callback(
			[&]()
			{	// 结束抖动,重置位置
				_is_shaking = false;
				reset();
			}
		);
	}
	~Camera() = default;

	void shake(float strength, int duration)
	{
		_is_shaking = true;
		_shaking_strength = strength;

		_timer_shake.set_wait_time(duration);
		_timer_shake.restart();
	}

	void reset()
	{
		_position.x = 0;
		_position.y = 0;
	}

	void on_update(int interval_ms)
	{
		_timer_shake.on_update(interval_ms);	// debug: 别忘了更新
		if (_is_shaking)
		{
			reset();
			// (-1, 1) * _shake_strength
			float x = (rand() % 100 - 50) / 50.0f * _shaking_strength;  
			float y = (rand() % 100 - 50) / 50.0f * _shaking_strength;
			_position.x += x;
			_position.y += y;
		}
	}

	// 窗口坐标 = 世界坐标 - 摄像机坐标
	const Vector2& get_position() const
	{
		return _position;
	}


};