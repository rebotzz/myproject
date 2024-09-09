#include <cmath>
#include "player.h"
#include "resources_manager.h"
#include "player_state_node.h"
#include "bullet_time_manager.h"
#include "audio_manager.h"

#include <iostream>
using std::cout;
using std::endl;

Player::Player() :Character()
{
	// 角色朝向,位置,高度初始化
	hp = 7;
	is_facing_left = false;
	position = { 200, 300 };
	logic_height = 90.0f;

	// 角色碰撞箱初始化
	hurt_box->set_enabled(true);
	hurt_box->set_size({ 40, 80 });
	hurt_box->set_layer_src(CollisionLayer::Player);
	hurt_box->set_layer_dst(CollisionLayer::None);
	hurt_box->set_position(get_logic_center());
	hurt_box->set_on_collision([&] {
		decrease_hp();
		});

	hit_box->set_enabled(false);
	hit_box->set_size({ 130,130 });
	hit_box->set_layer_src(CollisionLayer::None);
	hit_box->set_layer_dst(CollisionLayer::Enemy);
	hit_box->set_position(get_logic_center());

	// 攻击,翻滚定时器初始化
	timer_attack_cd.set_one_shot(true);
	timer_attack_cd.set_wait_time(ATTACK_CD);
	timer_attack_cd.set_on_timeout([&]()
		{
			is_attack_cd_comp = true;
		});
	timer_roll_cd.set_one_shot(true);
	timer_roll_cd.set_wait_time(ROLL_CD);
	timer_roll_cd.set_on_timeout([&]()
		{
			is_roll_cd_comp = true;
		});

	timer_bullet_time.set_one_shot(false);
	timer_bullet_time.set_wait_time(0.01f);
	timer_bullet_time.set_on_timeout([&]()
		{
			if (is_bullet_time)
				current_bullet_time -= 0.01f;
			else
				current_bullet_time += 0.01f;

			if (current_bullet_time < 0) 
				current_bullet_time = 0;
			if (current_bullet_time > BULLET_TIME_TOTAL) 
				current_bullet_time = BULLET_TIME_TOTAL;
		});

	// 动画初始化
	{
		// 角色动画
		AnimationGroup& idle = animation_pool["idle"];
		Animation& idle_left = idle.left;
		idle_left.set_interval(0.15f);
		idle_left.set_loop(true);
		idle_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		idle_left.add_frame(ResourcesManager::instance()->find_image("player_idle_left"), 5);

		Animation& idle_right = idle.right;
		idle_right.set_interval(0.15f);
		idle_right.set_loop(true);
		idle_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		idle_right.add_frame(ResourcesManager::instance()->find_image("player_idle_right"), 5);
	}
	{
		AnimationGroup& attack = animation_pool["attack"];
		Animation& attack_left = attack.left;
		attack_left.set_interval(0.05f);
		attack_left.set_loop(false);
		attack_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		attack_left.add_frame(ResourcesManager::instance()->find_image("player_attack_left"), 5);
		attack_left.set_on_finished([&]() { is_attacking = false; });

		Animation& attack_right = attack.right;
		attack_right.set_interval(0.05f);
		attack_right.set_loop(false);
		attack_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		attack_right.add_frame(ResourcesManager::instance()->find_image("player_attack_right"), 5);
		attack_right.set_on_finished([&]() { is_attacking = false; });
	}
	{
		AnimationGroup& run = animation_pool["run"];
		Animation& run_left = run.left;
		run_left.set_interval(0.075f);
		run_left.set_loop(true);
		run_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		run_left.add_frame(ResourcesManager::instance()->find_image("player_run_left"), 10);

		Animation& run_right = run.right;
		run_right.set_interval(0.075f);
		run_right.set_loop(true);
		run_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		run_right.add_frame(ResourcesManager::instance()->find_image("player_run_right"), 10);
	}
	{
		AnimationGroup& roll = animation_pool["roll"];
		Animation& roll_left = roll.left;
		roll_left.set_interval(0.05f);
		roll_left.set_loop(false);
		roll_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		roll_left.add_frame(ResourcesManager::instance()->find_image("player_roll_left"), 7);
		roll_left.set_on_finished([&]() { is_rolling = false; });

		Animation& roll_right = roll.right;
		roll_right.set_interval(0.05f);
		roll_right.set_loop(false);
		roll_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		roll_right.add_frame(ResourcesManager::instance()->find_image("player_roll_right"), 7);
		roll_right.set_on_finished([&]() { is_rolling = false; });
	}
	{
		AnimationGroup& jump = animation_pool["jump"];
		Animation& jump_left = jump.left;
		jump_left.set_interval(0.15f);
		jump_left.set_loop(false);
		jump_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		jump_left.add_frame(ResourcesManager::instance()->find_image("player_jump_left"), 5);

		Animation& jump_right = jump.right;
		jump_right.set_interval(0.15f);
		jump_right.set_loop(false);
		jump_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		jump_right.add_frame(ResourcesManager::instance()->find_image("player_jump_right"), 5);
	}
	{
		AnimationGroup& fall = animation_pool["fall"];
		Animation& fall_left = fall.left;
		fall_left.set_interval(0.15f);
		fall_left.set_loop(false);
		fall_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		fall_left.add_frame(ResourcesManager::instance()->find_image("player_fall_left"), 5);

		Animation& fall_right = fall.right;
		fall_right.set_interval(0.15f);
		fall_right.set_loop(false);
		fall_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		fall_right.add_frame(ResourcesManager::instance()->find_image("player_fall_right"), 5);
	}
	{
		AnimationGroup& dead = animation_pool["dead"];
		Animation& dead_left = dead.left;
		dead_left.set_interval(0.1f);
		dead_left.set_loop(false);
		dead_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		dead_left.add_frame(ResourcesManager::instance()->find_image("player_dead_left"), 6);

		Animation& dead_right = dead.right;
		dead_right.set_interval(0.1f);
		dead_right.set_loop(false);
		dead_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		dead_right.add_frame(ResourcesManager::instance()->find_image("player_dead_right"), 6);
	}
	{
		// 特效动画
		animation_vfx_slash_left.set_interval(0.07f);
		animation_vfx_slash_left.set_loop(false);
		animation_vfx_slash_left.set_achor_mode(Animation::AchorMode::Centered);
		animation_vfx_slash_left.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_left"), 5);

		animation_vfx_slash_right.set_interval(0.07f);
		animation_vfx_slash_right.set_loop(false);
		animation_vfx_slash_right.set_achor_mode(Animation::AchorMode::Centered);
		animation_vfx_slash_right.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_right"), 5);

		animation_vfx_slash_up.set_interval(0.07f);
		animation_vfx_slash_up.set_loop(false);
		animation_vfx_slash_up.set_achor_mode(Animation::AchorMode::Centered);
		animation_vfx_slash_up.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_up"), 5);

		animation_vfx_slash_down.set_interval(0.07f);
		animation_vfx_slash_down.set_loop(false);
		animation_vfx_slash_down.set_achor_mode(Animation::AchorMode::Centered);
		animation_vfx_slash_down.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_down"), 5);

		animation_vfx_jump.set_interval(0.05f);
		animation_vfx_jump.set_loop(false);
		animation_vfx_jump.set_achor_mode(Animation::AchorMode::BottomCentered);
		animation_vfx_jump.add_frame(ResourcesManager::instance()->find_image("player_vfx_jump"), 5);
		animation_vfx_jump.set_on_finished([&]() { is_vfx_jump_visiable = false; });

		animation_vfx_land.set_interval(0.1f);
		animation_vfx_land.set_loop(false);
		animation_vfx_land.set_achor_mode(Animation::AchorMode::BottomCentered);
		animation_vfx_land.add_frame(ResourcesManager::instance()->find_image("player_vfx_land"), 2);
		animation_vfx_land.set_on_finished([&]() { is_vfx_land_visiable = false; });
	}

	{
		// 状态机初始化
		state_machine.register_state("idle", new PlayerIdleState);
		state_machine.register_state("jump", new PlayerJumpState);
		state_machine.register_state("fall", new PlayerFallState);
		state_machine.register_state("run", new PlayerRunState);
		state_machine.register_state("roll", new PlayerRollState);
		state_machine.register_state("dead", new PlayerDeadState);
		state_machine.register_state("attack", new PlayerAttackState);

		// debug: 这里不能switch_to,不然调用的CharacterManager构造时调用Player构造,死循环
		state_machine.set_entry("idle");
	}
}

Player::~Player() = default;

void Player::on_input(const ExMessage& msg)
{
	if (hp < 0)
		return;

	static const int VK_A = 0x41;
	static const int VK_W = 0x57;
	static const int VK_S = 0x53;
	static const int VK_D = 0x44;

	static const int VK_J = 0x4A;
	static const int VK_K = 0x4B;

	static const int VK_F = 0x46;
	static const int VK_G = 0x47;

	// todo: 优化按键响应
	// 按键松开判断会有遗漏

	// 解决方案: 
	// 1.更改按键映射,全部改为键盘操作					可以改为空洞骑士按键
	// 2.更改别的接收键盘/鼠标消息的接口,不用EasyX的	用SDL? or window API?


	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case VK_A:
			is_left_key_down = true;
			break;
		case VK_W:
			is_jump_key_down = true;
			break;
		case VK_S:
			is_roll_key_down = true;
			break;
		case VK_D:
			is_right_key_down = true;
			break;

			// 临时方案:
		case VK_J:
			is_attack_key_down = true;
			attack_dir = (is_facing_left ? AttackDir::Left : AttackDir::Right);
			if (is_jump_key_down) attack_dir = AttackDir::Up;
			else if (is_roll_key_down) attack_dir = AttackDir::Down;
			break;
		case VK_K:
			AudioManager::instance()->play_audio_ex(_T("bullet_time"));
			BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Enter);
			break;
		}
		break;

	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case VK_A:
			is_left_key_down = false;
			break;
		case VK_W:
			is_jump_key_down = false;
			break;
		case VK_S:
			is_roll_key_down = false;
			break;
		case VK_D:
			is_right_key_down = false;
			break;

			// 临时方案:
		case VK_J:
			is_attack_key_down = false;
			break;
		case VK_K:
			BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Exit);
			break;

		}
		break;

	case WM_LBUTTONDOWN:
		is_attack_key_down = true;
		update_attack_dir(msg.x, msg.y);
		break;
	case WM_RBUTTONDOWN:
		if (current_bullet_time > 0)
			is_bullet_time = true;
		break;
	case WM_LBUTTONUP:
		//is_attack_key_down = false;			// debug:为了确保每次攻击能执行,这里不取消
		break;
	case WM_RBUTTONUP:
		is_bullet_time = false;
		break;
	}
}

void Player::on_update(float delta)
{
	// 角色速度,方向更新
	if (hp > 0 && !is_rolling)
		velocity.x = get_move_axis() * SPEED_RUN;
	if (get_move_axis() != 0 && !is_rolling)
		is_facing_left = get_move_axis() < 0;

	// 更新定时器
	timer_attack_cd.on_update(delta);
	timer_roll_cd.on_update(delta);
	timer_bullet_time.on_update(delta);

	// 更新动画
	animation_vfx_jump.on_update(delta);
	animation_vfx_land.on_update(delta);

	if (is_attacking)
	{
		timer_attack_cd.on_update(delta);
		current_slash_animation->set_position(get_logic_center());
		current_slash_animation->on_update(delta);
	}

	// 基类更新,动画,物理模拟
	Character::on_update(delta);

	// 状态栏更新
	status_bar.on_update(delta);

	// 子弹时间更新逻辑
	if (is_bullet_time && current_bullet_time > 0 && hp > 0)
	{
		AudioManager::instance()->play_audio_ex(_T("bullet_time"));
		BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Enter);
	}
	else
		BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Exit);
}

void Player::on_render()
{
	// 跳跃,落地特效
	if (is_vfx_jump_visiable)
		animation_vfx_jump.on_render();
	if (is_vfx_land_visiable)
		animation_vfx_land.on_render();

	// 攻击特效
	if (is_attacking)
		current_slash_animation->on_render();

	// 玩家动画
	Character::on_render();

	// 状态栏更新
	status_bar.on_render();
}

void Player::on_jump()
{
	velocity.y -= SPEED_JUMP;
	is_vfx_jump_visiable = true;
	animation_vfx_jump.reset();
	animation_vfx_jump.set_position(position);
}

void Player::on_land()
{
	is_vfx_land_visiable = true;
	animation_vfx_land.reset();
	animation_vfx_land.set_position(position);
}

void Player::on_roll()
{
	timer_roll_cd.restart();
	is_roll_cd_comp = false;
	velocity.x = is_facing_left ? -SPEED_ROLL : SPEED_ROLL;
}

void Player::on_hurt()
{
	AudioManager::instance()->play_audio_ex(_T("player_hurt"));
}

void Player::on_attack()
{
	is_attack_key_down = false;

	timer_attack_cd.restart();
	is_attack_cd_comp = false;
	switch (attack_dir)
	{
	case AttackDir::Up:
		current_slash_animation = &animation_vfx_slash_up;
		break;
	case AttackDir::Down:
		current_slash_animation = &animation_vfx_slash_down;
		break;
	case AttackDir::Left:
		current_slash_animation = &animation_vfx_slash_left;
		break;
	case AttackDir::Right:
		current_slash_animation = &animation_vfx_slash_right;
		break;
	}
	current_slash_animation->set_position(get_logic_center());
	current_slash_animation->reset();
}

void Player::update_attack_dir(float mouse_x, float mouse_y)
{
	static const float PI = 3.1415926535f;
	float angle = atan2(mouse_y - position.y, mouse_x - position.x);

	if (angle >= -PI / 4 && angle < PI / 4)
	{
		attack_dir = AttackDir::Right;
		is_facing_left = false;
	}
	else if (angle >= PI / 4 && angle < (PI * 3 / 4))
		attack_dir = AttackDir::Down;
	else if (angle >= (-PI * 3 / 4) && angle < -PI / 4)
		attack_dir = AttackDir::Up;
	else
	{
		attack_dir = AttackDir::Left;
		is_facing_left = true;
	}
}