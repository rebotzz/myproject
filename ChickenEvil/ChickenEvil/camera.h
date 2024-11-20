#pragma once
#include "vector2.h"
#include "timer.h"

class Camera
{
private:
	Vector2 pos_cur;				// 当前摄像机位置
	Vector2 pos_base;				// 基准摄像机位置
	Vector2 size;					// 窗口大小
	bool shake = false;				// 是否抖动
	int shake_strength = 0;			// 抖动强度
	Timer timer_shake;				// 抖动计时器

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

	// 窗口坐标 = 世界坐标 - 摄像机坐标
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