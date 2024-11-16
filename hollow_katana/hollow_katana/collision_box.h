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
	CollisionLayer layer_src = CollisionLayer::None;		// 受击层
	CollisionLayer layer_dst = CollisionLayer::None;		// 攻击层
	std::function<void()> on_collision;
	bool enabled = true;

	// 其他
	CollisionLayer trigger_layer = CollisionLayer::None;	// 记录发生碰撞的层,用于拼刀判定等

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

	const Vector2 get_position() const
	{
		return position;
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

	// add
	bool check_collision_layer_dst(CollisionLayer target_layer) const
	{
		return (int)layer_dst & (int)target_layer;
	}
	CollisionLayer get_trigger_layer() const 
	{ 
		return trigger_layer; 
	}
	void set_trigger_layer(CollisionLayer layer) 
	{ 
		trigger_layer = CollisionLayer::None | layer;
	}



};