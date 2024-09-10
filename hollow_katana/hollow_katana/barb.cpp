#include "barb.h"
#include "resources_manager.h"
#include "collision_manager.h"
#include "character_manager.h"
#include "audio_manager.h"


Barb::Barb(const Vector2& position)
{
	diff_period = (float)random_range(0, 6);
	base_position = position;
	current_position = position;

	// 碰撞箱初始化
	collision_box = CollisionManager::instance()->create_collision_box();
	collision_box->set_enabled(true);
	collision_box->set_layer_src(CollisionLayer::Enemy);
	collision_box->set_layer_dst(CollisionLayer::Player);
	collision_box->set_size({ 25, 25 });
	collision_box->set_on_collision([&]() { on_break(); });

	// 定时器初始化
	timer_idle.set_one_shot(true);
	timer_idle.set_wait_time((float)random_range(3, 10));
	timer_idle.set_on_timeout([&] 
		{ 
			if(status == Status::Idle)
			{
				status = Status::Aim;
				base_position = current_position;
			}
		});

	timer_aim.set_one_shot(true);
	timer_aim.set_wait_time(0.75f);
	timer_aim.set_on_timeout([&] 
		{ 
			if(status == Status::Aim)
			{
				status = Status::Dash;
				const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
				velocity = (pos_player - current_position).normalize() * SPEED_DASH;
			}
		});

	// 动画初始化
	{
		animation_loose.add_frame(ResourcesManager::instance()->find_atlas("barb_loose"));
		animation_loose.set_interval(0.15f);
		animation_loose.set_achor_mode(Animation::AchorMode::Centered);
		animation_loose.set_loop(true);
	}
	{
		animation_break.add_frame(ResourcesManager::instance()->find_atlas("barb_break"));
		animation_break.set_interval(0.1f);
		animation_break.set_achor_mode(Animation::AchorMode::Centered);
		animation_break.set_loop(false);
		animation_break.set_on_finished([&]() { is_valid = false; });
	}
}

Barb::~Barb()
{
	CollisionManager::instance()->destroy_collision_box(collision_box);
}

void Barb::on_update(float delta)
{
	// 更新定时器逻辑
	if (status == Status::Idle)
		timer_idle.on_update(delta);
	else if (status == Status::Aim)
		timer_aim.on_update(delta);

	// 更新移动逻辑
	total_delta_time += delta;
	switch (status)
	{
	case Status::Idle:
		current_position.x = base_position.x + sin(total_delta_time * 2 + diff_period) * 30;
		break;
	case Status::Aim:
		current_position.y = base_position.y + random_range(-10, 10);
		break;
	case Status::Dash:
		current_position += velocity * delta;
		if (current_position.y >= CharacterManager::instance()->get_player()->get_floor_y())
			on_break();
		if (current_position.y <= 0.0f)
			is_valid = false;
		break;
	}
	collision_box->set_position(current_position);

	// 更新动画逻辑
	current_animation = (status == Status::Break ? &animation_break : &animation_loose);
	current_animation->set_position(current_position);
	current_animation->on_update(delta);
}

void Barb::on_render()
{
	current_animation->on_render();
}

void Barb::on_break()
{
	if (status == Status::Break)
		return;

	status = Status::Break;
	collision_box->set_enabled(false);
	AudioManager::instance()->play_audio_ex(_T("barb_break"));
}