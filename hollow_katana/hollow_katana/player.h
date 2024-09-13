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
	const float SPEED_JUMP_MAX = 780.0f;
	const float SPEED_ROLL = 800.0f;
	const float ROLL_CD = 0.95f;
	const float ATTACK_CD = 0.7f;

	const float BULLET_TIME_TOTAL = 2.0f;							// 子弹时间最大值
	const float SPEED_DISPLACE_AXIS = 300.0f;						// 特殊位移(水平)速度
	const float SPEED_DISPLACE_UP_MAX = 780.0f;						// 特殊位移(上)速度最大值

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
	bool is_dance_key_down = false;									// 跳舞键是否按下

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
	bool is_bullet_time_key_down = false;							// 是否进入子弹时间
	float current_bullet_time = 2.0f;								// 当前剩余的子弹时间

	// 外加功能所需的定时器
	Direction beat_displace_dir = Direction::Left;					// 特殊位移方向
	bool is_displace_ex = false;									// 是否特殊位移
	Timer timer_enable_displace_ex;
	Timer timer_displace_ex;										// 角色特殊位移(攻击)定时器
	float speed_displace_up = 780.0f;								// 特殊位移(上)速度


	// 武器击中后坐力
	Timer timer_recoiling;											// 后坐力计时器
	float speed_jump = 780.0f;										// 跳跃高度控制

	Timer timer_hit_effect;											// 击中效果定时器
	bool is_hitting = false;										// 击中效果状态
	bool is_hit_cd_comp = true;										// 击中效果CD

	Timer timer_create_particle_effect;

private:
	StatusBar status_bar;

public:
	Player();
	~Player();

	virtual void on_hurt() override;
	virtual void on_input(const ExMessage& msg) override;
	virtual void on_update(float delta) override;
	virtual void on_render() override;

	void on_jump(float ratio = 1.0f);
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
	bool can_dance() const { return is_on_floor() && is_dance_key_down; }
	int get_move_axis() const { return is_right_key_down - is_left_key_down; }
	Direction get_attack_dir() const { return attack_dir; }

	// add
	float get_total_bullet_time() const { return BULLET_TIME_TOTAL; }
	float get_current_bullet_time() const { return current_bullet_time; }
	Direction get_facing_redir() const { return is_facing_left ? Direction::Right : Direction::Left; }
	float get_stay_air_time() { return 2 * speed_jump / GRAVITY; }
	void enable_displace_ex(Direction dir, float delta);
	void update_hit_box_position();
	void reset();


private:
	void on_hit_collide();
	// 击中后坐力
	void on_recoil(float delta = 0.0f);
	// 朝攻击方向冲刺一段距离
	void on_attack_displace_front();
	void create_hit_effect();
	void create_hurt_effect();
	void create_bullet_time_effect();
	void create_roll_effect();

private:
	void update_attack_dir(float mouse_x, float mouse_y);
	void update_attack_dir();
};