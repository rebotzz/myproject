#pragma once
#include "vector2.h"

class Camera
{
private:
	Vector2 _position;				// �����λ��
	Vector2 _size;					// ���ڴ�С

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

	// �������� = �������� - ���������
	void look_at(const Vector2& target)
	{
		_position = target - _size / 2.0f;
	}

	// todo
	void shake()
	{

	}

};