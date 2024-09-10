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
	CollisionLayer layer_src = CollisionLayer::None;	// 受击层
	CollisionLayer layer_dst = CollisionLayer::None;	// 攻击层
	std::function<void()> on_collision;
	bool enabled = true;

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

	bool check_hit_collision_layer(CollisionLayer target_layer)
	{
		if ((int)layer_dst & (int)CollisionLayer::None)
			return false;
		
		return (int)layer_dst & (int)target_layer;
	}
};