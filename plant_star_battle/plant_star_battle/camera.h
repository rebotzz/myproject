#pragma once
#include "vector2.h"

class Camera
{
private:
	Vector2 _position;
public:
	Camera() = default;
	~Camera() = default;

	void reset()
	{
		_position._x = 0;
		_position._y = 0;
	}

	void on_update(int interval_ms)
	{
		// for test
		const Vector2 speed = { -0.35f, 0};
		_position += speed * interval_ms;
	}

	const Vector2& get_position() const
	{
		return _position;
	}
};