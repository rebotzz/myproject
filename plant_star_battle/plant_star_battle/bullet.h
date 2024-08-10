#pragma once
#include <functional>
#include <easyx.h>
#include "vector2.h"
#include "player_id.h"
#include "camera.h"

// 子弹基类
class Bullet
{
protected:
	Vector2 _size;						// 子弹尺寸
	Vector2 _position;					// 子弹位置
	Vector2 _velocity;					// 子弹速度
	int _damage = 10;					// 子弹伤害
	bool _valid = true;					// 子弹碰撞检测是否有效
	bool _can_remove = false;			// 子弹是否移除
	std::function<void()> _callback;	// 子弹碰撞后回调逻辑
	PlayerID _target_id = PlayerID::P1;	// 子弹碰撞目标玩家id,区分敌我

public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int val)
	{
		_damage = val;
	}

	int get_damage() const
	{
		return _damage;
	}

	void set_size(float x, float y)
	{
		_size.x = x, _size.y = y;
	}

	const Vector2& get_size() const
	{
		return _size;
	}

	void set_collide_target(PlayerID target)
	{
		_target_id = target;
	}

	PlayerID get_collide_target() const
	{
		return _target_id;
	}

	void set_velocity(float x, float y)
	{
		_velocity.x = x;
		_velocity.y = y;
	}

	const Vector2& get_velocity() const
	{
		return _velocity;
	}

	void set_position(float x, float y)
	{
		_position.x = x;
		_position.y = y;
	}

	const Vector2& get_position() const
	{
		return _position;
	}

	void set_valid(bool flag)
	{
		_valid = flag;
	}

	bool get_valid() const
	{
		return _valid;
	}

	bool check_can_remove() const
	{
		return _can_remove;
	}

	void set_callback(std::function<void()> callback)
	{
		_callback = callback;
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		// 判定子弹中心是否在玩家碰撞矩形内
		float bullet_mid_x = _position.x + _size.x / 2;
		float bullet_mid_y = _position.y + _size.y / 2;
		return bullet_mid_x > position.x && bullet_mid_x < position.x + size.x
			&& bullet_mid_y > position.y && bullet_mid_y < position.y + size.y;
	}

	virtual void on_collide()
	{
		if (_callback)
			_callback();
	}

	virtual void on_draw(const Camera& camera) const
	{}

	virtual void on_update(int interval_ms)
	{}

protected:

	// 检测是否超过屏幕边界,避免子弹一直存在
	bool check_if_exceeds_screen(const Camera& camera)
	{
		Vector2 pos_camera = camera.get_position();
		return _position.x + _size.x - pos_camera.x < 0 || _position.x - pos_camera.x > getwidth()
			|| _position.y - pos_camera.y > getheight() || _position.y + _size.y - pos_camera.y < 0;
	}

};