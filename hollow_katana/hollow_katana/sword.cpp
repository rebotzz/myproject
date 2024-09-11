#include "sword.h"
#include "resources_manager.h"
#include "collision_manager.h"


Sword::Sword(const Vector2& postion_src, bool move_left)
{
	this->_position = postion_src;
	this->velocity = { (move_left ? -SPEED_MOVE : SPEED_MOVE), 0 };

	// 初始化碰撞箱
	collision_box = CollisionManager::instance()->create_collision_box();
	collision_box->set_enabled(true);
	collision_box->set_position(_position);
	collision_box->set_layer_src(CollisionLayer::Rebound);
	collision_box->set_layer_dst(CollisionLayer::Player);
	collision_box->set_size({ 190, 10 });

	// 初始动画
	animation.add_frame(ResourcesManager::instance()->find_atlas(move_left ? "sword_left" : "sword_right"));
	animation.set_achor_mode(Animation::AchorMode::Centered);
	animation.set_interval(0.1f);
	animation.set_loop(true);
	animation.set_position(_position);
}

Sword::~Sword()
{
	CollisionManager::instance()->destroy_collision_box(collision_box);
}

void Sword::on_update(float delta)
{
	_position += velocity * delta;
	collision_box->set_position(_position);
	animation.set_position(_position);

	animation.on_update(delta);

	if (_position.x < -200.0f || _position.x > getwidth() + 200.0f)
	{
		is_valid = false;
	}
}

void Sword::on_render()
{
	animation.on_render();
}