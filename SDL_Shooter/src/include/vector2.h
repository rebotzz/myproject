#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

class Vector2
{
public:
	Vector2() = default;
	Vector2(double x, double y) : x(x), y(y) {}
	Vector2(int x, int y) : x(x), y(y) {}
	Vector2(int angle) : x(cos(static_cast<double>(angle % 360) * 3.1415926 / 180.0)), 
						y(sin(static_cast<double>(angle % 360) * 3.1415926 / 180.0)){}
	Vector2(double radian) : x(cos(radian)), y(sin(radian)){}
	~Vector2() = default;

	Vector2 normalize() const
	{
		double len = lenth();
		if (len == 0) return *this;
		return Vector2(x / lenth(), y / lenth());
	}

	double lenth() const
	{
		return sqrt(x * x + y * y);
	}

	bool approach(const Vector2& vec) const
	{
		return (*this - vec).lenth() < 0.0001;
	}

	double angle() const
	{
		return atan2(y, x) * 180 / 3.1415926;	// 角度
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

	// 点乘
	double operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(double val) const
	{
		return { x * val, y * val };
	}

	Vector2 operator/(double val) const
	{
		if (val == 0) return { 0,0 };
		return { x / val, y / val };
	}

public:
	double x = 0;
	double y = 0;
};




#endif // _VECTOR2_H_