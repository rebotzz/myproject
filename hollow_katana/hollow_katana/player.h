#pragma once
#include "character.h"
#include "status_bar.h"

// 玩家类
class Player : public Character
{
public:
	enum class Direction
	{
		Up, Down, Left, Right
	};

private:
	const float SPEED_RUN = 300.0f;
	const float SPEED_JUMP = 780.0f;
	const float SPEED_ROLL = 800.0f;
	const float ROLL_CD = 0.95f;
	const float ATTACK_CD = 0.7f;

	const float BULLET_TIME_TOTAL = 2.0f;
	const float SPEED_DISPLACE_AXIS = 300.0f;
	const float SPEED_DISPLACE_UP = 670.0f;
	const float RECOIL_CD = 0.4f;					// todo:抖动bug,可能是多个碰撞箱子的问题
	//const float TIME_ZERO_GRAVITY = 0.15f;

private:
	Timer timer_roll_cd;											// 翻滚冷却时间定时器
	bool is_rolling = false;										// 是否正在翻滚
	bool is_roll_cd_comp = true;									// 翻滚冷却时间是否结束

	Timer timer_attack_cd;											// 攻击冷却时间定时器
	bool is_attacking = false;										// 是否正在攻击
	bool is_attack_cd_comp = true;									// 攻击冷却时间是否结束

	bool is_left_key_down = false;									// 向左移动键是否按下
	bool is_right_key_down = false;									// 向右移动键是否按下
	bool is_jump_key_down = false;									// 跳跃键是否按下
	bool is_roll_key_down = false;									// 翻滚键是否按下
	bool is_attack_key_down = false;								// 攻击键是否按下

	Animation animation_vfx_slash_up;								// 攻击刀光特效动画,上
	Animation animation_vfx_slash_down;								// 攻击刀光特效动画,下
	Animation animation_vfx_slash_left;								// 攻击刀光特效动画,左
	Animation animation_vfx_slash_right;							// 攻击刀光特效动画,右
	Direction attack_dir = Direction::Left;							// 攻击朝向
	Animation* current_slash_animation = nullptr;					// 当前刀光动画

	Animation animation_vfx_jump;									// 跳跃特效动画
	Animation animation_vfx_land;									// 落地特效动画
	bool is_vfx_jump_visiable = false;								// 跳跃特效动画是否可见
	bool is_vfx_land_visiable = false;								// 落地特效动画是否可见

	Timer timer_bullet_time;										// 子弹时间定时器
	bool is_bullet_time = false;									// 是否进入子弹时间
	float current_bullet_time = 2.0f;								// 当前剩余的子弹时间


	// 外加功能所需的定时器
	Timer timer_displace_ex;										// 角色特殊位移(攻击)定时器
	bool is_displace_ex = false;									// 是否特殊位移
	bool is_displace_ex_cd_comp = true;						
	Timer timer_enable_displace_ex;

	// 武器后坐力触发的CD
	Timer timer_recoil_cd;
	bool is_recoil_cd_comp = true;
	//Timer timer_zero_gravity;										// 为了强化下劈效果,短暂失重
		
	Direction beat_displace_dir = Direction::Left;					// 特殊位移方向

private:
	StatusBar status_bar;

public:
	Player();
	~Player();

	virtual void on_hurt() override;
	virtual void on_input(const ExMessage& msg) override;
	virtual void on_update(float delta) override;
	virtual void on_render() override;

	void on_jump();
	void on_land();
	void on_roll();
	void on_attack();

	void set_rolling(bool flag) { is_rolling = flag; }
	bool get_rolling() const { return is_rolling; }
	bool can_roll() const { return is_on_floor() && !is_rolling && is_roll_cd_comp && is_roll_key_down; }

	void set_attacking(bool flag) { is_attacking = flag; }
	bool get_attacking() const { return is_attacking; }
	bool can_attack() const { return !is_attacking && is_attack_cd_comp && is_attack_key_down; }

	bool can_jump() const { return is_on_floor() && is_jump_key_down; }
	int get_move_axis() const { return is_right_key_down - is_left_key_down; }
	Direction get_attack_dir() const { return attack_dir; }

	// my add
	float get_total_bullet_time() const { return BULLET_TIME_TOTAL; }
	float get_current_bullet_time() const { return current_bullet_time; }
	void enable_beat_displace(Direction dir, float delta)
	{
		beat_displace_dir = dir;
		timer_enable_displace_ex.set_wait_time(delta);
		timer_enable_displace_ex.restart();
		is_displace_ex = true;
	}
	void on_recoil()
	{
		if (!is_recoil_cd_comp)
			return;

		is_recoil_cd_comp = false;
		float duration = 0.1f;

		switch (attack_dir)
		{
		case Direction::Left:
			beat_displace_dir = Direction::Right;
			break;
		case Direction::Right:
			beat_displace_dir = Direction::Left;
			break;
		case Direction::Down:
			beat_displace_dir = Direction::Up;
			duration = 0.15f;
			//set_gravity_enable(false);
			//timer_zero_gravity.restart();
			break;
		}

		enable_beat_displace(beat_displace_dir, duration);
	}

	// 朝攻击方向冲刺一段距离
	void on_attack_displace_front()
	{
		if (is_displace_ex || attack_dir == Direction::Down)
			return;

		if(is_on_floor())
			enable_beat_displace(attack_dir, 0.08f);
		else
			enable_beat_displace(attack_dir, 0.15f);
	}

private:
	void update_attack_dir(float mouse_x, float mouse_y);

};