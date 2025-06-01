#pragma once

#include <cmath>

class Vector2
{
public:
	Vector2() = default;
	Vector2(float x, float y) : x(x), y(y) {}
	~Vector2() = default;

	Vector2 normalize() const
	{
		float len = lenth();
		if (len == 0) return *this;
		return Vector2(x / lenth(), y / lenth());
	}

	float lenth() const
	{
		return sqrt(x * x + y * y);
	}

	bool approach(const Vector2& vec) const
	{
		return (*this - vec).lenth() < 0.0001;
	}

	float angle() const
	{
		return atan2(y, x) * 180 / 3.1415926;	// ½Ç¶È
	}

	const Vector2& operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	const Vector2& operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vector2 operator+(const Vector2& vec) const
	{
		return { x + vec.x, y + vec.y };
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return { x - vec.x, y - vec.y };
	}

	// µã³Ë
	float operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(float val) const
	{
		return { x * val, y * val };
	}

	Vector2 operator/(float val) const
	{
		if (val == 0) return { 0,0 };
		return { x / val, y / val };
	}

public:
	float x = 0;
	float y = 0;
};

