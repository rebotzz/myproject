#pragma once
#include "vector2.h"
#include "timer.h"

class Camera
{
private:
	Vector2 _position;				// �����λ��
	Timer _timer_shake;				// �����������ʱ��
	float _shaking_strength = 0;	// �������������
	bool _is_shaking = false;		// ������Ƿ񶶶�

public:
	Camera()
	{
		_timer_shake.set_one_shot(true);
		_timer_shake.set_on_timeout(
			[&]()
			{	// ��������,����λ��
				_is_shaking = false;
				reset();
			}
		);
	}
	~Camera() = default;

	void shake(float strength, float duration)
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

	void on_update(float interval_ms)
	{
		_timer_shake.on_update(interval_ms);	// debug: �����˸���
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

	// �������� = �������� - ���������
	const Vector2& get_position() const
	{
		return _position;
	}

	void set_position(const Vector2& position)
	{
		_position = position;
	}

};