#include <cmath>
#include <memory>
#include "player.h"
#include "resources_manager.h"
#include "player_state_node.h"
#include "bullet_time_manager.h"
#include "audio_manager.h"
#include "particle_manager.h"
#include "effect.h"


#include <iostream>


Player::Player() :Character()
{
	// 角色朝向,位置,高度初始化
	hp = hp_max = 5;
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
		// 改用延时扣血,便于检测拼刀无敌状态,或许?
		timer_delay_decrease_hp.restart();
		//decrease_hp();
		});

	hit_box->set_enabled(false);
	hit_box->set_size({ 130,130 });
	hit_box->set_layer_src(CollisionLayer::None);
	hit_box->set_layer_dst(CollisionLayer::Enemy | CollisionLayer::Rebound | CollisionLayer::Sword);
	hit_box->set_position(get_logic_center());
	hit_box->set_on_collision([&](){
			// 武器击中后坐力,粒子特效,拼刀
			on_hit_collide();
		});

	interact_box->set_enabled(true);
	interact_box->set_size({ 40, 80 });
	interact_box->set_layer_src(CollisionLayer::None);
	interact_box->set_layer_dst(CollisionLayer::Interact | CollisionLayer::Scenery);
	interact_box->set_position(get_logic_center());
	interact_box->set_on_collision([&]()
		{

		});

	// 攻击,翻滚定时器初始化
	timer_attack_cd.set_one_shot(true);
	timer_attack_cd.set_wait_time(ATTACK_CD);
	timer_attack_cd.set_on_timeout([&]()
		{
			is_attack_cd_comp = true;
			is_down_slash = false;
		});
	timer_roll_cd.set_one_shot(true);
	timer_roll_cd.set_wait_time(ROLL_CD);
	timer_roll_cd.set_on_timeout([&]()
		{
			is_roll_cd_comp = true;
		});

	// 子弹时间定时器
	timer_bullet_time.set_one_shot(false);
	timer_bullet_time.set_wait_time(0.01f);
	timer_bullet_time.set_on_timeout([&]()
		{
			if (hp <= 0)
				is_bullet_time_key_down = false;

			// 子弹时间更新逻辑
			if (is_bullet_time_key_down)
				current_bullet_time -= 0.01f;
			else
				current_bullet_time += 0.01f;

			if (current_bullet_time < 0)
				current_bullet_time = 0;
			if (current_bullet_time > BULLET_TIME_TOTAL)
				current_bullet_time = BULLET_TIME_TOTAL;
		});

	// 特殊位移定时器
	timer_displace_ex.set_one_shot(false);
	timer_displace_ex.set_wait_time(0.01f);
	timer_displace_ex.set_on_timeout([&]
		{
			if (!is_displace_ex)
				return;
			
			// 死亡时的快速击退位移
			float ratio = hp > 0 ? 1.0f : 3.0f;

			switch (beat_displace_dir)
			{
			case Direction::Up:
				velocity.y -= 0.01f * GRAVITY * 1 / 2;
				position.y -= 0.01f * SPEED_DISPLACE_UP;
				break;
			case Direction::Left:
				position.x -= 0.01f * SPEED_DISPLACE_AXIS * ratio;
				break;
			case Direction::Right:
				position.x += 0.01f * SPEED_DISPLACE_AXIS * ratio;
				break;
			}
		});
	timer_enable_displace_ex.set_one_shot(true);
	timer_enable_displace_ex.set_wait_time(0.1f);
	timer_enable_displace_ex.set_on_timeout([&]
		{
			is_displace_ex = false;
		});
	timer_hit_effect.set_one_shot(true);
	timer_hit_effect.set_wait_time(ATTACK_CD * 0.8f);
	timer_hit_effect.set_on_timeout([&]
		{
			is_hit_eff_cd_comp = true;
		});

	// 粒子特效发射器
	timer_create_particle_effect.set_one_shot(false);
	timer_create_particle_effect.set_wait_time(0.06f);
	timer_create_particle_effect.set_on_timeout([&]
		{
			if (is_bullet_time_key_down)
				create_bullet_time_effect();
			else
				create_roll_effect();
		});

	// 为了拼刀机制加入的延时受伤定时器,效果未知
	timer_delay_decrease_hp.set_one_shot(true);
	timer_delay_decrease_hp.set_wait_time(0.001f);
	timer_delay_decrease_hp.set_on_timeout([&]
		{
			static bool init = true;
			if (init)
				init = false;
			else
				decrease_hp();
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
		attack_left.set_on_finished([&]() { 
			is_attacking = false; 
			is_hitting = false;
			});

		Animation& attack_right = attack.right;
		attack_right.set_interval(0.05f);
		attack_right.set_loop(false);
		attack_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		attack_right.add_frame(ResourcesManager::instance()->find_image("player_attack_right"), 5);
		attack_right.set_on_finished([&]() { 
			is_attacking = false; 
			is_hitting = false;
			});
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
		AnimationGroup& dead = animation_pool["dance"];
		Animation& dead_left = dead.left;
		dead_left.set_interval(0.1f);
		dead_left.set_loop(true);
		dead_left.set_achor_mode(Animation::AchorMode::BottomCentered);
		dead_left.add_frame(ResourcesManager::instance()->find_atlas("player_dance_left"));

		Animation& dead_right = dead.right;
		dead_right.set_interval(0.1f);
		dead_right.set_loop(true);
		dead_right.set_achor_mode(Animation::AchorMode::BottomCentered);
		dead_right.add_frame(ResourcesManager::instance()->find_atlas("player_dance_right"));
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
		state_machine.register_state("dance", new PlayerDanceState);

		// debug: 这里不能switch_to,不然调用的CharacterManager构造时调用Player构造,死循环
		state_machine.set_entry("idle");
	}
}

Player::~Player() = default;

void Player::on_input(const ExMessage& msg)
{
	if (hp <= 0)
		return;

	static const int VK_A = 0x41;
	static const int VK_W = 0x57;
	static const int VK_S = 0x53;
	static const int VK_D = 0x44;

	static const int VK_J = 0x4A;
	static const int VK_K = 0x4B;
	static const int VK_L = 0x4C;

	static const int VK_F = 0x46;
	static const int VK_G = 0x47;
	static const int VK_R = 0x52;

	static const int VK_Z = 0x5A;
	static const int VK_X = 0x58;
	static const int VK_C = 0x43;
	static const int VK_V = 0x56;

	// todo: 优化按键响应
	// 鼠标点击后,键盘按键松开判断会有遗漏	我很好奇,为什么别人的不会? 
	// 奇怪的一点是:将easyx.h改为老版的graphics.h后,按键遗漏似乎好了一点

	// 猜测的解决方案: 
	// 1.更改按键映射,全部改为键盘操作	-->效果不错
	// 2.更改别的接收键盘/鼠标消息的接口,不用EasyX的,用SDL? or windows API?	-->未实践

	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case VK_A:
		case VK_LEFT:
			is_left_key_down = true;
			break;
		case VK_W:
		case VK_UP:
		case VK_SPACE:
			is_jump_key_down = true;
			break;
		case VK_S:
		case VK_X:
		case VK_DOWN:
			is_roll_key_down = true;
			break;
		case VK_D:
		case VK_RIGHT:
			is_right_key_down = true;
			break;
		case VK_R:
		case VK_V:
			is_dance_key_down = true;
			break;
		case VK_J:
		case VK_Z:
			is_attack_key_down = true;
			update_attack_dir();
			break;
		case VK_K:
		case VK_C:
			is_bullet_time_key_down = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case VK_A:
		case VK_LEFT:
			is_left_key_down = false;
			break;
		case VK_W:
		case VK_UP:
		case VK_SPACE:
			is_jump_key_down = false;
			break;
		case VK_S:
		case VK_X:
		case VK_DOWN:
			is_roll_key_down = false;
			break;
		case VK_D:
		case VK_RIGHT:
			is_right_key_down = false;
			break;
		case VK_J:
		case VK_Z:
			is_attack_key_down = false;
			break;
		case VK_K:
		case VK_C:
			is_bullet_time_key_down = false;
			break;
		case VK_R:
		case VK_V:
			is_dance_key_down = false;
			break;
		}
		break;
	// 鼠标攻击方案现在还有bug,暂时弃用
	case WM_LBUTTONDOWN:
		is_attack_key_down = true;
		update_attack_dir(msg.x, msg.y);
		break;
	case WM_LBUTTONUP:
		is_attack_key_down = false;
		break;
	// 子弹时间键位2
	case WM_RBUTTONDOWN:
		if (current_bullet_time > 0)
			is_bullet_time_key_down = true;
		break;
	case WM_RBUTTONUP:
		is_bullet_time_key_down = false;
		break;
	default:
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
	timer_hit_effect.on_update(delta);
	timer_enable_displace_ex.on_update(delta);
	timer_displace_ex.on_update(delta);
	timer_delay_decrease_hp.on_update(delta);

	// 更新动画
	animation_vfx_jump.on_update(delta);
	animation_vfx_land.on_update(delta);

	if (is_attacking && hp > 0)
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
	if (is_bullet_time_key_down && current_bullet_time > 0.05f && hp > 0)
	{
		timer_create_particle_effect.on_update(delta);
		BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Enter);
	}
	else
		BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Exit);

	// 翻滚特效
	if(is_rolling)
		timer_create_particle_effect.on_update(delta);
}

void Player::on_render()
{
	// 跳跃,落地特效
	if (is_vfx_jump_visiable && hp > 0)
		animation_vfx_jump.on_render();
	if (is_vfx_land_visiable)
		animation_vfx_land.on_render();

	// 攻击刀光特效
	if (is_attacking && hp > 0)
		current_slash_animation->on_render();

	// 玩家动画
	Character::on_render();

	// 状态栏更新
	status_bar.on_render();
}

void Player::on_jump(float ratio)
{
	speed_jump = ratio * SPEED_JUMP_MAX; 
	velocity.y -= speed_jump;
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
	create_hurt_effect();
	main_camera->shake(4, 0.5);
}

void Player::on_attack()
{
	// 更新攻击CD,动画
	timer_attack_cd.restart();
	is_attack_cd_comp = false;
	switch (attack_dir)
	{
	case Direction::Up:
		current_slash_animation = &animation_vfx_slash_up;
		break;
	case Direction::Down:
		current_slash_animation = &animation_vfx_slash_down;
		break;
	case Direction::Left:
		current_slash_animation = &animation_vfx_slash_left;
		break;
	case Direction::Right:
		current_slash_animation = &animation_vfx_slash_right;
		break;
	}
	current_slash_animation->set_position(get_logic_center());
	current_slash_animation->reset();

	// 攻击冲刺位移
	on_attack_displace_front();
}

// [鼠标]攻击方向更新
void Player::update_attack_dir(float mouse_x, float mouse_y)
{
	static const float PI = 3.1415926535f;
	float angle = atan2(mouse_y - position.y, mouse_x - position.x);

	if (angle >= -PI / 4 && angle < PI / 4)
	{
		attack_dir = Direction::Right;
		is_facing_left = false;
	}
	else if (angle >= PI / 4 && angle < (PI * 3 / 4))
		attack_dir = Direction::Down;
	else if (angle >= (-PI * 3 / 4) && angle < -PI / 4)
		attack_dir = Direction::Up;
	else
	{
		attack_dir = Direction::Left;
		is_facing_left = true;
	}
}

// [键盘]攻击方向更新
void Player::update_attack_dir()
{
	attack_dir = (is_facing_left ? Direction::Left : Direction::Right);
	if (is_jump_key_down) attack_dir = Direction::Up;
	else if (is_roll_key_down) attack_dir = Direction::Down;
}

void Player::update_hit_box_position()
{
	const Vector2& pos_center = get_logic_center();
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;

	switch (attack_dir)
	{
	case Player::Direction::Up:
		pos_hit_box = { pos_center.x, pos_center.y - size_hit_box.y / 2 };
		break;
	case Player::Direction::Down:
		pos_hit_box = { pos_center.x, pos_center.y + size_hit_box.y / 2 };
		break;
	case Player::Direction::Left:
		pos_hit_box = { pos_center.x - size_hit_box.x / 2, pos_center.y };
		break;
	case Player::Direction::Right:
		pos_hit_box = { pos_center.x + size_hit_box.x / 2, pos_center.y };
		break;
	}
	hit_box->set_position(pos_hit_box);
}


void Player::enable_displace_ex(Direction dir, float delta)
{
	beat_displace_dir = dir;
	timer_enable_displace_ex.set_wait_time(delta);
	timer_enable_displace_ex.restart();
	is_displace_ex = true;
}


void Player::on_hit_collide()
{
	// 在碰撞箱体开启期间只碰撞一次
	if (is_hitting || !is_hit_eff_cd_comp)
		return;

	is_hitting = true;
	is_hit_eff_cd_comp = false;
	if (attack_dir == Direction::Down)
		is_down_slash = true;

	timer_hit_effect.restart();
	on_recoil();
	create_hit_effect();

	// 拼刀无敌帧
	if ((int)(CollisionLayer::Sword & hit_box->get_trigger_layer()))
	{
		make_invulnerable(true, 0.45f);
		std::shared_ptr<EffectSwordHit> particle(new EffectSwordHit(is_facing_left));
		Vector2 pos_spark = get_logic_center();
		switch (attack_dir)
		{
		case Player::Direction::Up: pos_spark.y -= 100;
			break;
		case Player::Direction::Down:  pos_spark.y += 100;
			break;
		case Player::Direction::Left:  pos_spark.x -= 100;
			break;
		case Player::Direction::Right:  pos_spark.x += 100;
			break;
		}
		particle->set_position(pos_spark);
		ParticleManager::instance()->register_particle(particle);

		switch (random_range(1, 3))
		{
		case 1: 
			AudioManager::instance()->play_audio_ex(_T("sword_hit_1"));
			break;
		case 2:
			AudioManager::instance()->play_audio_ex(_T("sword_hit_2"));
			break;
		case 3:
			AudioManager::instance()->play_audio_ex(_T("sword_hit_3"));
			break;
		}
	}
}

void Player::on_recoil(float delta)
{
	// 后坐力位移持续时间
	float recoil_delta = delta == 0.0f ? 0.1f : delta;

	switch (attack_dir)
	{
	case Direction::Left:
		beat_displace_dir = Direction::Right;
		enable_displace_ex(beat_displace_dir, recoil_delta);
		break;
	case Direction::Right:
		beat_displace_dir = Direction::Left;
		enable_displace_ex(beat_displace_dir, recoil_delta);
		break;
	case Direction::Down:
		beat_displace_dir = Direction::Up;
		velocity.y = -SPEED_JUMP_MAX;
		break;
	}
}

void Player::on_attack_displace_front()
{
	if (is_displace_ex || attack_dir == Direction::Down)
		return;

	if (is_on_floor())
	{
		if(attack_dir == Direction::Left || attack_dir == Direction::Right)
			enable_displace_ex(attack_dir, 0.08f);
	}
	else
	{
		if (attack_dir == Direction::Left || attack_dir == Direction::Right)
			enable_displace_ex(attack_dir, 0.15f);
		else if(attack_dir == Direction::Up)
			enable_displace_ex(attack_dir, 0.13f);	// 只是跳跃没有速度,力量感,需要快速突进
	}
}


void Player::create_hit_effect()
{
	std::shared_ptr<EffectHit> particle(new EffectHit(is_facing_left ? true : false));

	const Vector2& pos_center = get_logic_center();
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_particle;

	switch (attack_dir)
	{
	case Player::Direction::Up:
		pos_particle = { pos_center.x, pos_center.y - size_hit_box.y * 4 / 5};
		break;
	case Player::Direction::Down:
		pos_particle = { pos_center.x, pos_center.y + size_hit_box.y * 4 / 5 };
		break;
	case Player::Direction::Left:
		pos_particle = { pos_center.x - size_hit_box.x * 4 / 5, pos_center.y };
		break;
	case Player::Direction::Right:
		pos_particle = { pos_center.x + size_hit_box.x * 4 / 5, pos_center.y };
		break;
	}
	particle->set_position(pos_particle);
	
	ParticleManager::instance()->register_particle(particle);
}

void  Player::create_hurt_effect()
{
	std::shared_ptr<EffectHurt> particle(new EffectHurt(!is_facing_left));
	Vector2 pos_particle = get_logic_center();
	pos_particle.x += !is_facing_left ? -20 : 20;
	particle->set_position(pos_particle);

	ParticleManager::instance()->register_particle(particle);
}

void Player::create_bullet_time_effect()
{
	std::shared_ptr<EffectBulletTime> particle(new EffectBulletTime);
	IMAGE& frame = (is_facing_left ? current_animation->left : current_animation->right).get_current_frame();
	particle->add_image(frame);
	particle->set_position(position);

	ParticleManager::instance()->register_particle(particle);
}

void Player::create_roll_effect()
{
	std::shared_ptr<EffectRoll> particle(new EffectRoll);
	IMAGE& frame = (is_facing_left ? current_animation->left : current_animation->right).get_current_frame();
	particle->add_image(frame);
	particle->set_position(position);

	ParticleManager::instance()->register_particle(particle);
}

void Player::reset()
{
	Character::reset();

	is_left_key_down = false;
	is_right_key_down = false;
	is_jump_key_down = false;
	is_roll_key_down = false;
	is_attack_key_down = false;
	is_dance_key_down = false;

	is_facing_left = false;
	is_attacking = false;
	is_rolling = false;
	is_hitting = false;
	current_animation = &animation_pool["idle"];

	hurt_box->set_enabled(true);
	hit_box->set_enabled(false);
}
