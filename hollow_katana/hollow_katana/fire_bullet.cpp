#include "fire_bullet.h"
#include "resources_manager.h"
#include "collision_manager.h"
#include "character_manager.h"


FireBullet::FireBullet()
{
	// 初始化碰撞箱
	collision_box = CollisionManager::instance()->create_collision_box();
	collision_box->set_enabled(true);
	collision_box->set_position(_position);
	collision_box->set_layer_src(CollisionLayer::Rebound);
	collision_box->set_layer_dst(CollisionLayer::Player);
	collision_box->set_size({ 40, 50 });

	// 初始动画
	animation.add_frame(ResourcesManager::instance()->find_atlas("fire_bullet"));
	animation.set_achor_mode(Animation::AchorMode::Centered);
	animation.set_interval(0.75f);
	animation.set_loop(true);
	animation.set_position(_position);

	timer_trigger.set_one_shot(true);
	timer_trigger.set_wait_time(1.5f);
	timer_trigger.set_on_timeout([&]
		{
			is_trigger = true;
			velocity = (CharacterManager::instance()->get_player()->get_position() - _position).normalize() * SPEED;
		});
}

FireBullet::~FireBullet()
{
	CollisionManager::instance()->destroy_collision_box(collision_box);
}
void FireBullet::on_update(float delta)
{
	timer_trigger.on_update(delta);
	if (is_trigger)
		_position += velocity * delta;

	collision_box->set_position(_position);
	animation.set_position(_position);

	animation.on_update(delta);

	if (_position.x < -30.f || _position.x > getwidth() + 30.f)
		is_valid = false;
	if (_position.y < -30.f)
		is_valid = false;
}
void FireBullet::on_render()
{
	animation.on_render();
}




FireDash::FireDash(const Vector2& postion_src, bool move_left)
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
	animation.add_frame(ResourcesManager::instance()->find_atlas(move_left ? "fire_bullet_left" : "fire_bullet_right"));
	animation.set_achor_mode(Animation::AchorMode::Centered);
	animation.set_interval(0.1f);
	animation.set_loop(true);
	animation.set_position(_position);
}

FireDash::~FireDash()
{
	CollisionManager::instance()->destroy_collision_box(collision_box);
}

void FireDash::on_update(float delta)
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

void FireDash::on_render()
{
	animation.on_render();
}