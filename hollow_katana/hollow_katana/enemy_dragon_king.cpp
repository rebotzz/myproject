//#include "enemy_dragon_king.h"
//#include "resources_manager.h"
//#include "audio_manager.h"
//#include "enemy_dragon_king_state_node.h"
//
//EnemyDragonKing::EnemyDragonKing() :Character()
//{
//	// 角色初始化
//	hp = hp_max = 10;
//	is_facing_left = true;
//	position = { 1050, 500 };
//	logic_height = 90;
//
//	// 碰撞箱初始化		TODO:敌人不同状态动画对应的受击碰撞箱大小不同
//	hurt_box->set_size({ 50, 80 });
//	hit_box->set_size({ 40, 80 });
//
//	hurt_box->set_layer_src(CollisionLayer::Enemy);
//	hurt_box->set_layer_dst(CollisionLayer::None);
//	hit_box->set_layer_src(CollisionLayer::None);
//	hit_box->set_layer_dst(CollisionLayer::Player);
//
//	hurt_box->set_on_collision([&] { decrease_hp(); });
//
//	// 角色动画初始化
//	{
//		{
//			AnimationGroup& animation_idle = animation_pool["idle"];
//			Animation& idle_left = animation_idle.left;
//			idle_left.set_interval(0.1f);
//			idle_left.set_loop(true);
//			idle_left.set_achor_mode(Animation::AchorMode::BottomCentered);
//			idle_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_idle_left"));
//
//			Animation& idle_right = animation_idle.right;
//			idle_right.set_interval(0.1f);
//			idle_right.set_loop(true);
//			idle_right.set_achor_mode(Animation::AchorMode::BottomCentered);
//			idle_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_idle_right"));
//		}
//		{
//			AnimationGroup& animation_jump = animation_pool["jump"];
//			Animation& jump_left = animation_jump.left;
//			jump_left.set_interval(0.1f);
//			jump_left.set_loop(false);
//			jump_left.set_achor_mode(Animation::AchorMode::BottomCentered);
//			jump_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_jump_left"));
//
//			Animation& jump_right = animation_jump.right;
//			jump_right.set_interval(0.1f);
//			jump_right.set_loop(false);
//			jump_right.set_achor_mode(Animation::AchorMode::BottomCentered);
//			jump_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_jump_right"));
//		}
//		{
//			AnimationGroup& animation_fall = animation_pool["fall"];
//			Animation& fall_left = animation_fall.left;
//			fall_left.set_interval(0.1f);
//			fall_left.set_loop(false);
//			fall_left.set_achor_mode(Animation::AchorMode::BottomCentered);
//			fall_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_fall_left"));
//
//			Animation& fall_right = animation_fall.right;
//			fall_right.set_interval(0.1f);
//			fall_right.set_loop(false);
//			fall_right.set_achor_mode(Animation::AchorMode::BottomCentered);
//			fall_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_fall_right"));
//		}
//		{
//			AnimationGroup& animation_dash_on_floor = animation_pool["dash"];
//			Animation& dash_on_floor_left = animation_dash_on_floor.left;
//			dash_on_floor_left.set_interval(0.05f);
//			dash_on_floor_left.set_loop(true);
//			dash_on_floor_left.set_achor_mode(Animation::AchorMode::BottomCentered);
//			dash_on_floor_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_dash_left"));
//
//			Animation& dash_on_floor_right = animation_dash_on_floor.right;
//			dash_on_floor_right.set_interval(0.05f);
//			dash_on_floor_right.set_loop(true);
//			dash_on_floor_right.set_achor_mode(Animation::AchorMode::BottomCentered);
//			dash_on_floor_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_dash_right"));
//		}
//		{
//			AnimationGroup& animation_run = animation_pool["run"];
//			Animation& run_left = animation_run.left;
//			run_left.set_interval(0.05f);
//			run_left.set_loop(true);
//			run_left.set_achor_mode(Animation::AchorMode::BottomCentered);
//			run_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_run_left"));
//
//			Animation& run_right = animation_run.right;
//			run_right.set_interval(0.05f);
//			run_right.set_loop(true);
//			run_right.set_achor_mode(Animation::AchorMode::BottomCentered);
//			run_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_run_right"));
//		}
//
//
//
//
//	}
//
//	// 特效动画初始化
//	{
//		// todo: 武器刀光特效
//
//		//Animation& animation_dash_in_air_left = animation_dash_in_air_vfx.left;
//		//animation_dash_in_air_left.set_interval(0.1f);
//		//animation_dash_in_air_left.set_loop(true);
//		//animation_dash_in_air_left.set_achor_mode(Animation::AchorMode::BottomCentered);
//		//animation_dash_in_air_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_vfx_dash_left"));
//
//		//Animation& animation_dash_in_air_right = animation_dash_in_air_vfx.right;
//		//animation_dash_in_air_right.set_interval(0.1f);
//		//animation_dash_in_air_right.set_loop(true);
//		//animation_dash_in_air_right.set_achor_mode(Animation::AchorMode::BottomCentered);
//		//animation_dash_in_air_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_vfx_dash_right"));
//	
//	}
//
//	// 状态机初始化
//	{
//		//state_machine.register_state("idle", new EnemyDragonKingIdleState);
//		//state_machine.register_state("jump", new EnemyDragonKingJumpState);
//		//state_machine.register_state("run", new EnemyDragonKingRunState);
//		//state_machine.register_state("fall", new EnemyDragonKingFallState);
//		//state_machine.register_state("dash_in_air", new EnemyDragonKingDashState);
//		//state_machine.register_state("aim", new EnemyDragonKingAimState);
//		//state_machine.register_state("dead", new EnemyDragonKingDeadState);
//
//		state_machine.set_entry("idle");
//	}
//}
//
//EnemyDragonKing::~EnemyDragonKing()
//{
//	//AudioManager::instance()->stop_audio_ex(_T("enemy_dragon_kingrun"));
//}