#pragma once

#include "vector2.h"

#include <functional>

enum class CollisionLayer
{
	None = 0,
	Enemy,			// ����
	Home,			// home
	Prop,			// ����
};

class CollisionMgr;		// ��ǰ�������Ͳ��ð���.h������ѭ������

class CollisionBox
{
	friend class CollisionMgr;

public:
	// ��ײ����״
	enum class ShapeMode
	{
		Rect,				// ����
		Circle				// Բ��
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
	Vector2 postion;										// ����λ��
	Vector2 size;											// ��ײ���γߴ�
	CollisionLayer src_layer = CollisionLayer::None;		// �����ܱ���ײ�Ĳ㼶
	CollisionLayer dst_layer = CollisionLayer::None;		// ��Ҫ��ײ��Ŀ��㼶
	std::function<void()> collide_callback;					// ��ײ�����߼�
	bool is_valid = true;

	ShapeMode shape_mode = ShapeMode::Rect;					// ��ײ����
	float radius = 0;										// ��ײԲ�ΰ뾶
	void* user_data = nullptr;								// �����������Ϣ
	CollisionBox* on_collide_box = nullptr;					// ��֮����ײ����ײ��
};
