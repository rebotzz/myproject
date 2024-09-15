#include "fire_bullet.h"
#include "resources_manager.h"
#include "collision_manager.h"
#include "character_manager.h"
#include "audio_manager.h"


FireBullet::FireBullet()
{
	// 初始化碰撞箱
	collision_box = CollisionManager::instance()->create_collision_box();
	collision_box->set_enabled(false);
	collision_box->set_position(_position);
	collision_box->set_layer_src(CollisionLayer::Rebound);
	collision_box->set_layer_dst(CollisionLayer::Player);
	collision_box->set_size({ 40, 50 });
	collision_box->set_on_collision([&]()
		{
			if ((int)(collision_box->get_trigger_layer() & CollisionLayer::Rebound))
			{
				velocity.x = -velocity.x;
				velocity.y = -velocity.y;
				collision_box->set_enabled(false);
				//collision_box->set_layer_src(CollisionLayer::None);
				//collision_box->set_layer_dst(CollisionLayer::Enemy);
			}
		});

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
			collision_box->set_enabled(true);
			AudioManager::instance()->play_audio_ex(_T("fire_bullet"));
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

	if (_position.x < -200.f || _position.x > getwidth() + 200.f)
		is_valid = false;
	if (_position.y < -200.f)
		is_valid = false;
}
void FireBullet::on_render()
{
	animation.on_render();
}

