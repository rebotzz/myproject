#pragma once
#include "vector2.h"
#include "timer.h"

class Camera
{
private:
	Vector2 pos_cur;				// ��ǰ�����λ��
	Vector2 pos_base;				// ��׼�����λ��
	Vector2 size;					// ���ڴ�С
	bool shake = false;				// �Ƿ񶶶�
	int shake_strength = 0;			// ����ǿ��
	Timer timer_shake;				// ������ʱ��

public:
	Camera()
	{
		timer_shake.set_one_shot(false);
		timer_shake.set_wait_time(0.05f);
		timer_shake.set_on_timeout([&]()
			{
				pos_cur.x = pos_base.x + (rand() % 2 ? 1 : -1) * (rand() % shake_strength);
				pos_cur.y = pos_base.y + (rand() % 2 ? 1 : -1) * (rand() % shake_strength);
			});
	}
	~Camera() = default;

	void set_size(const Vector2& val)
	{
		size = val;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void set_position(const Vector2& val)
	{
		pos_base = pos_cur = val;
	}

	const Vector2& get_position() const
	{
		return pos_cur;
	}

	// �������� = �������� - ���������
	void look_at(const Vector2& target)
	{
		pos_cur = target - size / 2.0f;
	}

	void set_shake(bool enable = false, int strenth = 5)
	{
		shake = enable;
		shake_strength = strenth;
		if (!enable) pos_cur = pos_base;
	}

	void on_update(float delta)
	{
		if (shake)
			timer_shake.on_update(delta);
	}

};