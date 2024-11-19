#pragma once
#include "vector2.h"

class Camera
{
private:
	Vector2 _position;				// 摄像机位置
	Vector2 _size;					// 窗口大小

public:
	Camera() = default;
	~Camera() = default;

	void set_size(const Vector2& size)
	{
		_size = size;
	}

	const Vector2& get_size() const
	{
		return _size;
	}

	void set_position(const Vector2& position)
	{
		_position = position;
	}

	const Vector2& get_position() const
	{
		return _position;
	}

	// 窗口坐标 = 世界坐标 - 摄像机坐标
	void look_at(const Vector2& target)
	{
		_position = target - _size / 2.0f;
	}

	// todo
	void shake()
	{

	}

};