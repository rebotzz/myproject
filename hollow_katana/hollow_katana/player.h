#pragma once
#include "character.h"
#include "status_bar.h"

// 玩家类
class Player : public Character
{
public:
	enum class AttackDir
	{
		Up, Down, Left, Right
	};

	enum class BeatDisplaceDir
	{
		Up, Left, Right
	};

private:
	const float SPEED_RUN = 300.0f;
	const float SPEED_JUMP = 780.0f;
	const float SPEED_ROLL = 800.0f;
	const float ROLL_CD = 0.95f;
	const float ATTACK_CD = 0.7f;

	const float BULLET_TIME_TOTAL = 2.0f;
	const float SPEED_BEAT_DISPLACE = 300.0f;

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
	AttackDir attack_dir = AttackDir::Left;							// 攻击朝向
	Animation* current_slash_animation = nullptr;					// 当前刀光动画

	Animation animation_vfx_jump;									// 跳跃特效动画
	Animation animation_vfx_land;									// 落地特效动画
	bool is_vfx_jump_visiable = false;								// 跳跃特效动画是否可见
	bool is_vfx_land_visiable = false;								// 落地特效动画是否可见

	Timer timer_bullet_time;										// 子弹时间定时器
	bool is_bullet_time = false;									// 是否进入子弹时间
	float current_bullet_time = 2.0f;								// 当前剩余的子弹时间

	Timer timer_beating_displace;									// 角色特殊位移(因为打击而位移)定时器
	bool is_beating_displace = false;								// 是否特殊位移
	Timer timer_enable_beat_displace;
	float beating_displace_time = 0.3f;								// 特殊位移持续时间

	BeatDisplaceDir beat_displace_dir = BeatDisplaceDir::Left;		// 特殊位移方向

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
	bool can_roll() const { return !is_rolling && is_roll_cd_comp && is_roll_key_down; }

	void set_attacking(bool flag) { is_attacking = flag; }
	bool get_attacking() const { return is_attacking; }
	bool can_attack() const { return !is_attacking && is_attack_cd_comp && is_attack_key_down; }

	bool can_jump() const { return is_on_floor() && is_jump_key_down; }
	int get_move_axis() const { return is_right_key_down - is_left_key_down; }
	AttackDir get_attack_dir() const { return attack_dir; }

	// my add
	float get_total_bullet_time() const { return BULLET_TIME_TOTAL; }
	float get_current_bullet_time() const { return current_bullet_time; }
	void enable_beat_displace(BeatDisplaceDir dir, float delta)
	{
		beat_displace_dir = dir;
		timer_enable_beat_displace.set_wait_time(delta);
		timer_enable_beat_displace.restart();
	}

private:
	void update_attack_dir(float mouse_x, float mouse_y);

};