#pragma once
#include <cmath>

// 二维向量类
class Vector2
{
public:
	float x = 0.0f;
	float y = 0.0f;
public:
	Vector2() = default;
	Vector2(float x, float y)
		: x(x), y(y){}
	Vector2(int x, int y)
		: x((float)x), y((float)y) {}
	~Vector2() = default;

	// 二维向量运算 + - * / 长度len 标准化normalize
	void operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
	}

	void operator*=(float value)
	{
		x *= value;
		y *= value;
	}

	void operator/=(float value)
	{
		x /= value;
		y /= value;
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	float operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(float value) const
	{
		return Vector2(x * value, y * value);
	}

	Vector2 operator/(float value) const
	{
		return Vector2(x / value, y / value);
	}

	float length() const
	{
		return sqrt(x * x + y * y);
	}

	// 标准化分量
	Vector2 normalize() const 
	{
		float len = length();
		if (len == 0) return Vector2(0, 0);

		return Vector2(x / len, y / len);
	}

	// 矢量相等
	bool approx(const Vector2 target)
	{
		return (*this - target).length() <= 0.0001f;
	}

	// 与X轴的角度
	double angle() const
	{
		////方法1：
		//double val = (atan(y / x) * 180.0) / 3.141592654;			// atan值域(-PI/2, PI/2)：x正半轴
		//if ((x < 0 && y < 0) || (x < 0 && y > 0)) val -= 180.0;		// 增加映射范围：x负半轴
		//return val;

		return atan2(y, x) * 180.0 / 3.141592654;
	}

};