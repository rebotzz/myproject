#include "enemy_dragon_king.h"
#include "resources_manager.h"
#include "audio_manager.h"
#include "enemy_dragon_king_state_node.h"

EnemyDragonKing::EnemyDragonKing() :Character()
{
	// ��ɫ��ʼ��
	hp = hp_max = 10;
	is_facing_left = true;
	position = { 1050, 500 };
	logic_height = 90;

	// ��ɫ��ײ���ʼ��
	hurt_box->set_size({ 70, 100 });
	hit_box->set_size({ 40, 80 });

	hurt_box->set_layer_src(CollisionLayer::Enemy);
	hurt_box->set_layer_dst(CollisionLayer::None);
	hit_box->set_layer_src(CollisionLayer::None);
	hit_box->set_layer_dst(CollisionLayer::Player);
	hurt_box->set_on_collision([&] { decrease_hp(); });

	collision_box_katana = CollisionManager::instance()->create_collision_box();
	collision_box_katana->set_enabled(false);
	collision_box_katana->set_size({ 130,130 });
	collision_box_katana->set_layer_src(CollisionLayer::None);
	collision_box_katana->set_layer_dst(CollisionLayer::Player | CollisionLayer::Rebound);

	// ��ɫ������ʼ��
	{
		{
			AnimationGroup& animation_idle = animation_pool["idle"];
			Animation& idle_left = animation_idle.left;
			idle_left.set_interval(0.1f);
			idle_left.set_loop(true);
			idle_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			idle_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_idle_left"));

			Animation& idle_right = animation_idle.right;
			idle_right.set_interval(0.1f);
			idle_right.set_loop(true);
			idle_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			idle_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_idle_right"));
		}
		{
			AnimationGroup& animation_attack = animation_pool["attack"];
			Animation& attack_left = animation_attack.left;
			attack_left.set_interval(0.75f);
			attack_left.set_loop(false);
			attack_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			attack_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_attack_left"));

			Animation& attack_right = animation_attack.right;
			attack_right.set_interval(0.75f);
			attack_right.set_loop(false);
			attack_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			attack_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_attack_right"));
		}
		{
			AnimationGroup& run = animation_pool["run"];
			Animation& run_left = run.left;
			run_left.set_interval(0.075f);
			run_left.set_loop(true);
			run_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			run_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_run_left"));

			Animation& run_right = run.right;
			run_right.set_interval(0.075f);
			run_right.set_loop(true);
			run_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			run_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_run_right"));
		}

		{
			AnimationGroup& roll = animation_pool["roll"];
			Animation& roll_left = roll.left;
			roll_left.set_interval(0.05f);
			roll_left.set_loop(false);
			roll_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			roll_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_roll_left"));

			Animation& roll_right = roll.right;
			roll_right.set_interval(0.05f);
			roll_right.set_loop(false);
			roll_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			roll_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_roll_right"));
		}
		{
			AnimationGroup& jump = animation_pool["jump"];
			Animation& jump_left = jump.left;
			jump_left.set_interval(0.15f);
			jump_left.set_loop(false);
			jump_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			jump_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_jump_left"));

			Animation& jump_right = jump.right;
			jump_right.set_interval(0.15f);
			jump_right.set_loop(false);
			jump_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			jump_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_jump_right"));
		}
		{
			// ���ݽ�ɫ�ٶȺ�������������ʱ��,ȷ��֡���
			int frame_count = ResourcesManager::instance()->find_atlas("enemy_dragon_king_fall_1_left")->get_size();
			float interval = (SPEED_JUMP_BIG / GRAVITY) / (float)frame_count;

			AnimationGroup& fall = animation_pool["fall_big"];
			Animation& fall_left = fall.left;
			fall_left.set_interval(interval);
			fall_left.set_loop(false);
			fall_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			fall_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_fall_1_left"));

			Animation& fall_right = fall.right;
			fall_right.set_interval(interval);
			fall_right.set_loop(false);
			fall_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			fall_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_fall_1_right"));
		}
		{
			int frame_count = ResourcesManager::instance()->find_atlas("enemy_dragon_king_fall_2_left")->get_size();
			float interval = (SPEED_JUMP_SMALL / GRAVITY) / (float)frame_count;

			AnimationGroup& fall = animation_pool["fall_small"];
			Animation& fall_left = fall.left;
			fall_left.set_interval(interval);
			fall_left.set_loop(false);
			fall_left.set_achor_mode(Animation::AchorMode::BottomCentered);
			fall_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_fall_2_left"));

			Animation& fall_right = fall.right;
			fall_right.set_interval(interval);
			fall_right.set_loop(false);
			fall_right.set_achor_mode(Animation::AchorMode::BottomCentered);
			fall_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dragon_king_fall_2_right"));
		}
	}

	// ��Ч������ʼ��
	{
		// ��Ч����
		animation_vfx_slash_left.set_interval(0.07f);
		animation_vfx_slash_left.set_loop(false);
		animation_vfx_slash_left.set_achor_mode(Animation::AchorMode::Centered);
		animation_vfx_slash_left.add_frame(ResourcesManager::instance()->find_image("enemy_vfx_attack_left"), 5);

		animation_vfx_slash_right.set_interval(0.07f);
		animation_vfx_slash_right.set_loop(false);
		animation_vfx_slash_right.set_achor_mode(Animation::AchorMode::Centered);
		animation_vfx_slash_right.add_frame(ResourcesManager::instance()->find_image("enemy_vfx_attack_right"), 5);

		animation_vfx_slash_up.set_interval(0.07f);
		animation_vfx_slash_up.set_loop(false);
		animation_vfx_slash_up.set_achor_mode(Animation::AchorMode::Centered);
		animation_vfx_slash_up.add_frame(ResourcesManager::instance()->find_image("enemy_vfx_attack_up"), 5);

		animation_vfx_slash_down.set_interval(0.07f);
		animation_vfx_slash_down.set_loop(false);
		animation_vfx_slash_down.set_achor_mode(Animation::AchorMode::Centered);
		animation_vfx_slash_down.add_frame(ResourcesManager::instance()->find_image("enemy_vfx_attack_down"), 5);
	}

	// ״̬����ʼ��
	{

		state_machine.set_entry("idle");
	}
}

EnemyDragonKing::~EnemyDragonKing()
{
	AudioManager::instance()->stop_audio_ex(_T("player_run"));
	CollisionManager::instance()->destroy_collision_box(collision_box_katana);
}

void EnemyDragonKing::on_hurt()
{
	AudioManager::instance()->play_audio_ex(_T("player_hurt"));
}

void EnemyDragonKing::on_update(float delta)
{
	if (velocity.x > 0.0001f)
		is_facing_left = (velocity.x < 0);

	// ���»��������ģ����߼�
	Character::on_update(delta);

	// ������Ч��������ײ���߼�
	hit_box->set_position(get_logic_center());

	if (is_attacking)
	{
		current_slash_animation->set_position(get_logic_center());
		current_slash_animation->on_update(delta);
	}
}

void EnemyDragonKing::on_render()
{
	Character::on_render();

	// ����������Ч
	if (is_attacking && hp > 0)
		current_slash_animation->on_render();
}

