#pragma once

#include "vector2.h"

#include <functional>

enum class CollisionLayer
{
	None = 0,
	Enemy,			// 敌人
	Home,			// home
	Prop,			// 道具
};

class CollisionMgr;		// 提前声明，就不用包含.h，避免循环引用

class CollisionBox
{
	friend class CollisionMgr;

public:
	// 碰撞盒形状
	enum class ShapeMode
	{
		Rect,				// 矩形
		Circle				// 圆形
	};
public:
	void set_size(const Vector2& size)
	{
		this->size = size;
	}

	void set_position(const Vector2& postion)
	{
		this->postion = postion;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	const Vector2& get_postion() const
	{
		return postion;
	}

	void set_src_layer(CollisionLayer layer)
	{
		src_layer = layer;
	}

	void set_dst_layer(CollisionLayer layer)
	{
		dst_layer = layer;
	}

	void set_on_collide(const std::function<void()>& callback)
	{
		collide_callback = callback;
	}

	void set_valid(bool flag)
	{
		is_valid = flag;
	}

	bool get_valid() const
	{
		return is_valid;
	}

	void on_collide()
	{
		if (is_valid && collide_callback)
			collide_callback();
	}

	void set_user_data(void* user_data_ptr)
	{
		user_data = user_data_ptr;
	}

	void* get_user_data() const
	{
		return user_data;
	}

	const CollisionBox* get_on_collide_box() const
	{
		return on_collide_box;
	}

	void set_shape_mode(ShapeMode mode)
	{
		shape_mode = mode;
	}

	ShapeMode get_shape_mode() const
	{
		return shape_mode;
	}

	float get_radius() const
	{
		return radius;
	}

	void set_radius(float radius)
	{
		this->radius = radius;
	}

private:
	void set_on_collide_box(CollisionBox* on_collide_box)
	{
		this->on_collide_box = on_collide_box;
	}

private:
	CollisionBox() = default;
	CollisionBox(const CollisionBox&) = delete;
	const CollisionBox& operator=(const CollisionBox&) = delete;
	~CollisionBox() = default;

private:
	Vector2 postion;										// 中心位置
	Vector2 size;											// 碰撞矩形尺寸
	CollisionLayer src_layer = CollisionLayer::None;		// 自身能被碰撞的层级
	CollisionLayer dst_layer = CollisionLayer::None;		// 想要碰撞的目标层级
	std::function<void()> collide_callback;					// 碰撞处理逻辑
	bool is_valid = true;

	ShapeMode shape_mode = ShapeMode::Rect;					// 碰撞外形
	float radius = 0;										// 碰撞圆形半径
	void* user_data = nullptr;								// 持有者相关信息
	CollisionBox* on_collide_box = nullptr;					// 与之相碰撞的碰撞盒
};
