#pragma once
#include <functional>
#include "vector2.h"
#include "collision_layer.h"

class CollisionManager;

class CollisionBox
{
	friend class CollisionManager;
private:
	Vector2 size;
	Vector2 position;
	CollisionLayer layer_src = CollisionLayer::None;				// 受击层
	CollisionLayer layer_dst = CollisionLayer::None;				// 攻击层
	std::function<void()> on_collision;
	bool enabled = true;

	bool is_src_collision = false;
	bool is_dst_collision = false;


private:
	CollisionBox() = default;
	~CollisionBox() = default;

public:

	void set_size(const Vector2 size)
	{
		this->size = size;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	// 使用中心锚点模式
	void set_position(const Vector2 position)
	{
		this->position = position;
	}

	void set_layer_src(CollisionLayer layer_src)
	{
		this->layer_src = layer_src;
	}

	void set_layer_dst(CollisionLayer layer_dst)
	{
		this->layer_dst = layer_dst;
	}

	void set_on_collision(std::function<void()> on_collision)
	{
		this->on_collision = on_collision;
	}

	void set_enabled(bool flag)
	{
		enabled = flag;
	}

	// my add
	bool check_collision_layer_dst(CollisionLayer target_layer) const
	{
		return (int)layer_dst & (int)target_layer;
	}

	void set_dst_colliding(bool flag)
	{
		is_dst_collision = flag;
	}
	bool get_dst_colliding() const
	{
		return is_dst_collision;
	}
	void set_src_colliding(bool flag)
	{
		is_src_collision = flag;
	}
	bool get_src_colliding()
	{
		return is_src_collision;
	}

};