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
	collision_box->set_position(current_position);
	collision_box->set_layer_src(CollisionLayer::Rebound);	// 会不会自己碰到自己?
	collision_box->set_layer_dst(CollisionLayer::Player);
	collision_box->set_size({ 40, 50 });
	collision_box->set_on_collision([&]()
		{
			if ((int)(collision_box->get_trigger_layer() & (CollisionLayer::Rebound | CollisionLayer::Sword)))
			{
				// 或许也可以让子弹攻向boss
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
	animation.set_position(current_position);

	timer_trigger.set_one_shot(true);
	timer_trigger.set_wait_time(1.5f);
	timer_trigger.set_on_timeout([&]
		{
			is_trigger = true;
			velocity = (CharacterManager::instance()->get_player()->get_position() - current_position).normalize() * SPEED;
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
		current_position += velocity * delta;
	else
		update_circle(delta);	// 转圈圈

	collision_box->set_position(current_position);
	animation.set_position(current_position);
	animation.on_update(delta);

	if (current_position.x < -100.f || current_position.x > getwidth() + 100.f)
		is_valid = false;
	if (current_position.y < -100.f)
		is_valid = false;
}
void FireBullet::on_render()
{
	animation.on_render();
}

