#pragma once
#include <cmath>

// 二维向量类
class Vector2
{
public:
	float _x = 0.0f;
	float _y = 0.0f;
public:
	Vector2() = default;
	Vector2(float x, float y)
		: _x(x), _y(y){}
	~Vector2() = default;

	// 二维向量运算 + - *  len normalize
	void operator+=(const Vector2& vec)
	{
		_x += vec._x;
		_y += vec._y;
	}

	void operator-=(const Vector2& vec)
	{
		_x -= vec._x;
		_y -= vec._y;
	}

	void operator*=(float value)
	{
		_x *= value;
		_y *= value;
	}

	void operator/=(float value)
	{
		_x /= value;
		_y /= value;
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(_x - vec._x, _y - vec._y);
	}

	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(_x + vec._x, _y + vec._y);
	}

	float operator*(const Vector2& vec) const
	{
		return _x * vec._x + _y * vec._y;
	}

	Vector2 operator*(float value) const
	{
		return Vector2(_x * value, _y * value);
	}

	float length()
	{
		return sqrt(_x * _x + _y * _y);
	}

	// 标准化分量
	Vector2 normalize()
	{
		float len = length();
		if (len == 0) return Vector2(0, 0);

		return Vector2(_x / len, _y / len);
	}

};