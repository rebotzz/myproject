#pragma once
//#include <easyx.h>
#include <graphics.h>		//easyx图形库
#include "vector2.h"
#include "timer.h"
#include "animation.h"
#include "collision_manager.h"
#include "state_machine.h"
#include "control.h"
#include "game_object.h"

#include <iostream>


// 角色基类, 角色由状态机驱动
class Character : public GameObject
{
protected:
	struct AnimationGroup
	{
		Animation left;
		Animation right;
	};

protected:																
	const float FLOOR_Y = 620;											// 地面竖直高度y坐标
	const float GRAVITY = 980 * 2;										// 重力常数
	const float TIME_INVINCIBLE = 1.0f;									// 无敌帧时长
	const float SPEED_JUMP_MAX = 780.0f;
	float speed_jump = 780.0f;											// 跳跃高度控制
	const float SPEED_RUN = 300.0f;

protected:						
	int hp_max = 1;														// 生命值上限
	int hp = 1;															// 生命值
	float logic_height = 0.0f;											// 逻辑高度
	bool is_facing_left = false;										// 角色朝向
	bool enable_gravity = true;											// 是否开始重力
	Vector2 position;													// 角色位置,底部中心锚点模式
	Vector2 velocity;													// 速度
	CollisionBox* hurt_box = nullptr;									// 受击碰撞箱
	CollisionBox* hit_box = nullptr;									// 攻击碰撞箱
	CollisionBox* interact_box = nullptr;								// 交互碰撞箱
	StateMachine* state_machine = nullptr;								// 状态机
	Timer timer_invincible_status;										// 无敌状态计时器
	Timer timer_invincible_blink;										// 无敌时间闪烁计时器
	bool is_invincible_status = false;									// 是否无敌状态
	bool is_blink_invisible = false;									// 是否无敌闪烁的不可见状态
	bool not_blink = false;												// 不闪烁
	bool is_invisible = false;											// 是否可见,播放特效,隐藏本体
	AnimationGroup* current_animation = nullptr;						// 当前动画组
	std::unordered_map<std::string, AnimationGroup> animation_pool;		// 角色动画池

	Timer timer_attack_cd;												// 攻击冷却时间定时器
	bool is_attacking = false;											// 是否正在攻击
	bool is_attack_cd_comp = true;										// 攻击冷却时间是否结束

	Animation animation_vfx_jump;										// 跳跃特效动画
	Animation animation_vfx_land;										// 落地特效动画
	bool is_vfx_jump_visiable = false;									// 跳跃特效动画是否可见
	bool is_vfx_land_visiable = false;									// 落地特效动画是否可见

	// 跳跃平台
	// 平台地面高度,因为某一时刻只会踩在一个平台上;这么看来,角色可以自己创建平台，自己跳跃。而每个角色拥有独立平台地面高度
	float platform_floor_y = 720.0f;	
	Timer timer_platform_reset;											// 平台地面高度恢复定时器
	float prev_frame_pos_y = 0.0f;										// 上一帧角色Y轴坐标位置

	// 角色控制
	bool is_left_key_down = false;										// 向左移动键是否按下
	bool is_right_key_down = false;										// 向右移动键是否按下
	bool is_jump_key_down = false;										// 跳跃键是否按下
	bool is_dash_key_down = false;										// 冲刺/翻滚键是否按下
	bool is_attack_key_down = false;									// 攻击键是否按下
	bool is_skill_1_key_down = false;									// 1技能键是否按下
	bool is_skill_2_key_down = false;									// 2技能键是否按下
	Control* control = nullptr;											// 角色控制组件

	// TODO:如果声效ID放在这里，那么状态节点播放音效可以更通用
	std::string audio_run;


public:
	Character();
	~Character();

	// 核心接口
	virtual void on_input(const ExMessage& msg);
	virtual void on_update(float delta);
	virtual void on_render();

	void switch_state(const std::string& id);
	void make_invincible(bool not_blink_ = false, float delta_ratio = 1.0f);
	void set_animation(const std::string& id);
	void decrease_hp();

	virtual void reset() { hp = hp_max; }
	virtual void on_hurt() { };

	// getters and setters
	int get_hp_max() const { return hp_max; }
	int get_hp() const { return hp; }
	float get_gravity() const { return GRAVITY; }
	void set_hp(int val) { hp = val; }
	void set_gravity_enable(bool flag) { enable_gravity = flag; }
	void set_facing_left(bool flag) { is_facing_left = flag; }
	bool get_facing_left() const { return is_facing_left; }
	void set_invisible(bool flag) { is_invisible = flag; }
	Vector2 get_logic_center() const { return { position.x, position.y - logic_height / 2 }; }
	void set_position(const Vector2& position) { this->position = position; }
	const Vector2& get_position() const { return position; }
	void set_velocity(const Vector2& velocity) { this->velocity = velocity; }
	const Vector2& get_velocity() const { return velocity; }
	CollisionBox* get_hurt_box() const { return hurt_box; }
	CollisionBox* get_hit_box() const { return hit_box; }

	void set_hp_max(int val) { hp_max = val; }
	void set_attacking(bool flag) { is_attacking = flag; }
	bool get_attacking() const { return is_attacking; }
	bool can_attack() const { return !is_attacking && is_attack_cd_comp && is_attack_key_down; }
	bool can_jump() const { return is_on_floor() && is_jump_key_down; }

	// 角色控制
	void set_control(Control* control) { this->control = control; }
	Control* get_control() const { return control; }
	void set_left_keydown(bool flag) { is_left_key_down = flag; }
	void set_right_keydown(bool flag) { is_right_key_down = flag; }
	void set_jump_keydown(bool flag) { is_jump_key_down = flag; }
	void set_dash_keydown(bool flag) { is_dash_key_down = flag; }
	void set_attack_keydown(bool flag) { is_attack_key_down = flag; }
	void set_skill_1_keydown(bool flag) { is_skill_1_key_down = flag; }
	void set_skill_2_keydown(bool flag) { is_skill_2_key_down = flag; }
	bool get_left_keydown() { return is_left_key_down; }
	bool get_right_keydown() { return is_right_key_down; }
	bool get_jump_keydown() { return is_jump_key_down; }
	bool get_dash_keydown() { return is_dash_key_down; }
	bool get_attack_keydown() { return is_attack_key_down; }
	bool get_skill_1_keydown() { return is_skill_1_key_down; }
	bool get_skill_2_keydown() { return is_skill_2_key_down; }
	int get_move_axis() const { return is_right_key_down - is_left_key_down; }
	virtual void switch_to_human_control() {}
	virtual void switch_to_auto_control() {}

	virtual void on_jump(float ratio = 1.0f);
	virtual void on_land();
	virtual void move(float delta);

	// 技能释放
	virtual bool can_skill_1() const { return is_skill_1_key_down; }
	virtual bool can_skill_2() const { return is_skill_2_key_down; }
	virtual void release_skill_1() {}
	virtual void release_skill_2() {}
	virtual void release_attack() {}

	// 时间回溯2.0接口
	HistoryStatus get_current_status() const 
	{ 
		return (is_facing_left ? current_animation->left : current_animation->right).get_current_status();
	}

	void on_platform(bool flag) { if (flag) timer_platform_reset.restart(); }

	// 默认缺省参数：不启用平台地面高度;如果没站在平台上，一段时间后取消平台
	void set_platform_floor_y(float val = 720.0f) { platform_floor_y = val; }
	bool is_on_floor() const { return position.y >= FLOOR_Y || position.y >= platform_floor_y; }
	float get_floor_y() const { return min(FLOOR_Y, platform_floor_y); }
	float get_prev_frame_pos_y() const { return prev_frame_pos_y; }
};