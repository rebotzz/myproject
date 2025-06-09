#pragma once
#include "character.h"
#include "status_bar.h"


// 玩家类
class Player : public Character
{
	friend class PlayerControlAWSD;
	friend class PlayerControlArrow;

private:
	const float SPEED_ROLL = 800.0f;								// 翻滚速度
	const float ROLL_CD = 0.95f;									// 翻滚CD
	const float ATTACK_CD = 0.7f;									// 攻击CD
	const float BULLET_TIME_TOTAL = 2.0f;							// 子弹时间最大值
	const float SPEED_DISPLACE_AXIS = 300.0f;						// 特殊位移(水平)速度
	const float SPEED_DISPLACE_UP = 780.0f;							// 特殊位移(上)速度

private:
	Timer timer_roll_cd;											// 翻滚冷却时间定时器
	bool is_rolling = false;										// 是否正在翻滚
	bool is_roll_cd_comp = true;									// 翻滚冷却时间是否结束

	Animation animation_vfx_slash_up;								// 攻击刀光特效动画,上
	Animation animation_vfx_slash_down;								// 攻击刀光特效动画,下
	Animation animation_vfx_slash_left;								// 攻击刀光特效动画,左
	Animation animation_vfx_slash_right;							// 攻击刀光特效动画,右
	Direction attack_dir = Direction::Left;							// 攻击朝向
	Animation* current_slash_animation = nullptr;					// 当前刀光动画

	Timer timer_bullet_time;										// 子弹时间定时器
	bool is_bullet_time_key_down = false;							// 是否进入子弹时间
	float current_bullet_time = 2.0f;								// 当前剩余的子弹时间

	// 外加功能所需的定时器
	Direction beat_displace_dir = Direction::Left;					// 特殊位移方向
	bool is_displace_ex = false;									// 是否特殊位移
	Timer timer_enable_displace_ex;									// 特殊位移持续时间定时器
	Timer timer_displace_ex;										// 角色特殊位移(攻击)位置累计移动定时器
	bool is_down_slash = false;										// 是否下劈
	bool is_sword_hit = false;										// 是否拼刀
	Timer timer_delay_decrease_hp;									// 延时扣血用于检测是否在拼刀
	Timer timer_recoiling;											// 武器击中后坐力计时器
	Timer timer_hit_effect;											// 击中效果定时器
	bool is_hitting = false;										// 击中效果状态
	bool is_hit_eff_cd_comp = true;									// 击中效果CD
	Timer timer_create_particle_effect;								// 粒子特效发射器
	Vector2 pos_cursor = { 0, 0 };									// 十字瞄准图标(鼠标)位置
	IMAGE* img_crosshair = nullptr;									// 十字瞄准图标

	StatusBar status_bar;											// 状态栏
	bool is_control_preset_awsd = true;								// 是否启用按键预设1:AWSD按键

public:
	Player();
	~Player();

	virtual void on_input(const ExMessage& msg) override;
	virtual void on_update(float delta) override;
	virtual void on_render() override;

	virtual void on_hurt() override;
	void on_roll();
	void on_attack();
	virtual void move(float delta) override;

	// get与set方法
	void reset();
	bool can_roll() const { return is_on_floor() && !is_rolling && is_roll_cd_comp && is_dash_key_down; }
	bool can_dance() const { return is_on_floor() && is_skill_1_key_down; }
	void set_rolling(bool flag) { is_rolling = flag; }
	bool get_rolling() const { return is_rolling; }
	float get_total_bullet_time() const { return BULLET_TIME_TOTAL; }
	float get_current_bullet_time() const { return current_bullet_time; }
	Direction get_attack_dir() const { return attack_dir; }
	Direction get_facing_redir() const { return is_facing_left ? Direction::Right : Direction::Left; }
	float get_stay_air_time() { return 2 * speed_jump / GRAVITY; }
	void enable_displace_ex(Direction dir, float delta);
	void update_hit_box_position();

	// 按键预设切换
	void set_control_preset(bool flag) { is_control_preset_awsd = flag; }
	bool get_control_preset() { return is_control_preset_awsd; }
	void set_bullet_time_keydown(bool flag) { is_bullet_time_key_down = flag; }
	virtual bool can_skill_1() const override { return can_roll(); }
	virtual bool can_skill_2() const override { return can_dance(); }
	virtual void release_skill_1() { switch_state("roll"); }
	virtual void release_skill_2() { switch_state("dance"); }

private:
	void on_hit_collide();					// 攻击命中回调逻辑
	void on_recoil(float delta = 0.0f);		// 击中后坐力
	void on_attack_direction_move();		// 朝攻击方向冲刺一段距离
	void create_hit_effect();				// 攻击特效
	void create_hurt_effect();				// 受击特效
	void create_bullet_time_effect();		// 子弹时间残影特效
	void create_roll_effect();				// 翻滚特效
	void update_attack_dir();
	void update_attack_dir(float mouse_x, float mouse_y);
};