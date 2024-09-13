#include "enemy_hornet.h"
#include "collision_manager.h"
#include "resources_manager.h"
#include "audio_manager.h"
#include "enemy_hornet_state_node.h"

EnemyHornet::EnemyHornet() :Character()
{
	// 角色初始化
	hp = hp_max = 10;
	is_facing_left = true;
	position = { 1050, 500 };
	logic_height = 150;

	// 碰撞箱初始化		TODO:敌人不同状态动画对应的受击碰撞箱大小不同
	hurt_box->set_size({ 100, 150 });
	hit_box->set_size({ 50, 80 });

	hurt_box->set_layer_src(CollisionLayer::Enemy);
	hurt_box->set_layer_dst(CollisionLayer::None);
	hit_box->set_layer_src(CollisionLayer::None);
	hit_box->set_layer_dst(CollisionLayer::Player);

	hurt_box->set_on_collision([&] { decrease_hp(); });

	collision_box_silk = CollisionManager::instance()->create_collision_box();
	collision_box_silk->set_enabled(false);
	collision_box_silk->set_size({ 230, 230 });
	collision_box_silk->set_layer_src(CollisionLayer::None);
	collision_box_silk->set_layer_dst(CollisionLayer::Player);

	// 角色动画初始化
	{
		{
			AnimationGroup& animation_idle = animation_pool["idle"];
			Animation& idle_left = animation_idle.left;
			idle_left.set_interval(0.1f);
			idle_left.set_loop(true);
			idle_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			idle_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_idle_left"));

			Animation& idle_right = animation_idle.right;
			idle_right.set_interval(0.1f);
			idle_right.set_loop(true);
			idle_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			idle_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_idle_right"));
		}
		{
			AnimationGroup& animation_jump = animation_pool["jump"];
			Animation& jump_left = animation_jump.left;
			jump_left.set_interval(0.1f);
			jump_left.set_loop(false);
			jump_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			jump_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_jump_left"));

			Animation& jump_right = animation_jump.right;
			jump_right.set_interval(0.1f);
			jump_right.set_loop(false);
			jump_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			jump_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_jump_right"));
		}
		{
			AnimationGroup& animation_fall = animation_pool["fall"];
			Animation& fall_left = animation_fall.left;
			fall_left.set_interval(0.1f);
			fall_left.set_loop(false);
			fall_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			fall_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_fall_left"));

			Animation& fall_right = animation_fall.right;
			fall_right.set_interval(0.1f);
			fall_right.set_loop(false);
			fall_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			fall_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_fall_right"));
		}
		{
			AnimationGroup& animation_aim = animation_pool["aim"];
			Animation& aim_left = animation_aim.left;
			aim_left.set_interval(0.05f);
			aim_left.set_loop(false);
			aim_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			aim_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_aim_left"));

			Animation& aim_right = animation_aim.right;
			aim_right.set_interval(0.05f);
			aim_right.set_loop(false);
			aim_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			aim_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_aim_right"));
		}
		{
			AnimationGroup& animation_dash_in_air = animation_pool["dash_in_air"];
			Animation& dash_in_air_left = animation_dash_in_air.left;
			dash_in_air_left.set_interval(0.05f);
			dash_in_air_left.set_loop(true);
			dash_in_air_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			dash_in_air_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_dash_in_air_left"));

			Animation& dash_in_air_right = animation_dash_in_air.right;
			dash_in_air_right.set_interval(0.05f);
			dash_in_air_right.set_loop(true);
			dash_in_air_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			dash_in_air_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_dash_in_air_right"));
		}
		{
			AnimationGroup& animation_dash_on_floor = animation_pool["dash_on_floor"];
			Animation& dash_on_floor_left = animation_dash_on_floor.left;
			dash_on_floor_left.set_interval(0.05f);
			dash_on_floor_left.set_loop(true);
			dash_on_floor_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			dash_on_floor_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_dash_on_floor_left"));

			Animation& dash_on_floor_right = animation_dash_on_floor.right;
			dash_on_floor_right.set_interval(0.05f);
			dash_on_floor_right.set_loop(true);
			dash_on_floor_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			dash_on_floor_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_dash_on_floor_right"));
		}
		{
			AnimationGroup& animation_run = animation_pool["run"];
			Animation& run_left = animation_run.left;
			run_left.set_interval(0.05f);
			run_left.set_loop(true);
			run_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			run_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_run_left"));

			Animation& run_right = animation_run.right;
			run_right.set_interval(0.05f);
			run_right.set_loop(true);
			run_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			run_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_run_right"));
		}
		{
			AnimationGroup& animation_squat = animation_pool["squat"];
			Animation& squat_left = animation_squat.left;
			squat_left.set_interval(0.05f);
			squat_left.set_loop(true);
			squat_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			squat_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_squat_left"));

			Animation& squat_right = animation_squat.right;
			squat_right.set_interval(0.05f);
			squat_right.set_loop(true);
			squat_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			squat_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_squat_right"));
		}
		{
			AnimationGroup& animation_throw_barb = animation_pool["throw_barbs"];
			Animation& throw_barb_left = animation_throw_barb.left;
			throw_barb_left.set_interval(0.1f);
			throw_barb_left.set_loop(false);
			throw_barb_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			throw_barb_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_throw_barb_left"));

			Animation& throw_barb_right = animation_throw_barb.right;
			throw_barb_right.set_interval(0.1f);
			throw_barb_right.set_loop(false);
			throw_barb_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			throw_barb_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_throw_barb_right"));
		}
		{
			AnimationGroup& animation_throw_silk = animation_pool["throw_silk"];
			Animation& throw_silk_left = animation_throw_silk.left;
			throw_silk_left.set_interval(0.1f);
			throw_silk_left.set_loop(false);
			throw_silk_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			throw_silk_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_throw_silk_left"));

			Animation& throw_silk_right = animation_throw_silk.right;
			throw_silk_right.set_interval(0.1f);
			throw_silk_right.set_loop(false);
			throw_silk_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			throw_silk_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_throw_silk_right"));
		}
		{
			AnimationGroup& animation_throw_sword = animation_pool["throw_sword"];
			Animation& throw_sword_left = animation_throw_sword.left;
			throw_sword_left.set_interval(0.05f);
			throw_sword_left.set_loop(false);
			throw_sword_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			throw_sword_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_throw_sword_left"));

			Animation& throw_sword_right = animation_throw_sword.right;
			throw_sword_right.set_interval(0.05f);
			throw_sword_right.set_loop(false);
			throw_sword_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			throw_sword_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_throw_sword_right"));
		}
	}

	// 特效动画初始化
	{
		animation_silk.set_interval(0.1f);
		animation_silk.set_loop(true);
		animation_silk.set_achor_mode(Animation::AchorMode::Centered);
		animation_silk.add_frame(ResourcesManager::instance()->find_atlas("silk"));

		Animation& animation_dash_in_air_left = animation_dash_in_air_vfx.left;
		animation_dash_in_air_left.set_interval(0.1f);
		animation_dash_in_air_left.set_loop(true);
		animation_dash_in_air_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		animation_dash_in_air_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_vfx_dash_in_air_left"));

		Animation& animation_dash_in_air_right = animation_dash_in_air_vfx.right;
		animation_dash_in_air_right.set_interval(0.1f);
		animation_dash_in_air_right.set_loop(true);
		animation_dash_in_air_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		animation_dash_in_air_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_vfx_dash_in_air_right"));

		Animation& animation_dash_on_floor_left = animation_dash_on_floor_vfx.left;
		animation_dash_on_floor_left.set_interval(0.1f);
		animation_dash_on_floor_left.set_loop(true);
		animation_dash_on_floor_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		animation_dash_on_floor_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_vfx_dash_on_floor_left"));

		Animation& animation_dash_on_floor_right = animation_dash_on_floor_vfx.right;
		animation_dash_on_floor_right.set_interval(0.1f);
		animation_dash_on_floor_right.set_loop(true);
		animation_dash_on_floor_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		animation_dash_on_floor_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_hornet_vfx_dash_on_floor_right"));
	}

	// 状态机初始化
	{
		state_machine.register_state("idle", new EnemyHornetIdleState);
		state_machine.register_state("jump", new EnemyHornetJumpState);
		state_machine.register_state("run", new EnemyHornetRunState);
		state_machine.register_state("fall", new EnemyHornetFallState);
		state_machine.register_state("dash_in_air", new EnemyHornetDashInAirState);
		state_machine.register_state("dash_on_floor", new EnemyHornetDashOnFloorState);
		state_machine.register_state("aim", new EnemyHornetAimState);
		state_machine.register_state("squat", new EnemyHornetSquatState);
		state_machine.register_state("throw_sword", new EnemyHornetThrowSwordState);
		state_machine.register_state("throw_barbs", new EnemyHornetThrowBarbsState);
		state_machine.register_state("throw_silk", new EnemyHornetThrowSilkState);
		state_machine.register_state("dead", new EnemyHornetDeadState);

		state_machine.set_entry("idle");
	}
}

EnemyHornet::~EnemyHornet()
{
	CollisionManager::instance()->destroy_collision_box(collision_box_silk);

	for (Barb* barb : barb_list)
		delete barb;
	for (Sword* sword : sword_list)
		delete sword;

	AudioManager::instance()->stop_audio_ex(_T("enemy_run"));
}

void EnemyHornet::on_hurt()
{
	switch (random_range(1, 3))
	{
	case 1:
		AudioManager::instance()->play_audio_ex(_T("enemy_hurt_1"));
		break;
	case 2:
		AudioManager::instance()->play_audio_ex(_T("enemy_hurt_2"));
		break;
	case 3:
		AudioManager::instance()->play_audio_ex(_T("enemy_hurt_3"));
		break;
	}
}

void EnemyHornet::on_update(float delta)
{
	if (velocity.x > 0.0001f)
		is_facing_left = (velocity.x < 0);

	// 更新基类的物理模拟等逻辑
	Character::on_update(delta);

	// 更新特效动画和碰撞箱逻辑
	hit_box->set_position(get_logic_center());

	if (is_throwing_silk)
	{
		animation_silk.on_update(delta);
		animation_silk.set_position(get_logic_center());
		collision_box_silk->set_position(get_logic_center());
	}
	else
		collision_box_silk->set_enabled(false);

	if (is_dashing_in_air || is_dashing_on_floor)
	{
		current_dash_animation->on_update(delta);
		current_dash_animation->set_position(position + (is_dashing_in_air ? Vector2(0, 150) : Vector2(0, 0)));
	}

	// 道具更新逻辑
	for (Barb* barb : barb_list)
		barb->on_update(delta);
	for (Sword* sword : sword_list)
		sword->on_update(delta);

	// 清除无效的道具
	barb_list.erase(std::remove_if(barb_list.begin(), barb_list.end(),
		[](Barb* barb)
		{
			bool can_remove = !barb->check_valid();
			if (can_remove) delete barb;
			return can_remove;
		}), barb_list.end());

	sword_list.erase(std::remove_if(sword_list.begin(), sword_list.end(),
		[](Sword* sword)
		{
			bool can_remove = !sword->check_valid();
			if (can_remove) delete sword;
			return can_remove;
		}), sword_list.end());
}

void EnemyHornet::on_render()
{
	for (Barb* barb : barb_list)
		barb->on_render();
	for (Sword* sword : sword_list)
		sword->on_render();

	Character::on_render();

	if (is_dashing_in_air || is_dashing_on_floor)
		current_dash_animation->on_render();

	if (is_throwing_silk)
		animation_silk.on_render();
}

void EnemyHornet::throw_sword()
{
	Sword* sword = new Sword(get_logic_center(), is_facing_left);
	sword_list.push_back(sword);
}

void EnemyHornet::throw_barbs()
{
	int spawn_barb_num = random_range(3, 6);
	if (barb_list.size() > 10)
		spawn_barb_num = 1;
	int width_grid = getwidth() / spawn_barb_num;
	for (int i = 0; i < spawn_barb_num; ++i)
	{
		int rand_x = random_range(i * width_grid, (i + 1) * width_grid);
		int rand_y = random_range(250, 500);
		Barb* new_barb = new Barb({ (float)rand_x, (float)rand_y });
		barb_list.push_back(new_barb);
	}
}

void EnemyHornet::on_dash()
{
	AnimationGroup& animation_vfx = (is_dashing_on_floor ? animation_dash_on_floor_vfx : animation_dash_in_air_vfx);
	current_dash_animation = (is_facing_left ? &animation_vfx.left : &animation_vfx.right);
	current_dash_animation->reset();
}

void EnemyHornet::on_throw_silk()
{
	animation_silk.reset();
	collision_box_silk->set_enabled(true);
}

