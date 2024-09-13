#pragma once
#include <easyx.h>
#include "vector2.h"
#include "timer.h"
#include "animation.h"
#include "collision_manager.h"
#include "state_machine.h"

// 角色基类, 角色由状态机驱动
class Character
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
	const float TIME_INVULNERABLE = 1.0f;								// 无敌帧时长

protected:						
	int hp_max = 1;														// 生命值上限
	int hp = 1;															// 生命值
	float logic_height = 0.0f;											// 逻辑高度
	bool is_facing_left = false;										// 角色朝向
	bool enable_gravity = true;											// 是否开始重力
	Vector2 position;													// 角色位置,底部中心锚点模式
	Vector2 velocity;													// 速度
	CollisionBox* hurt_box;												// 受击碰撞箱
	CollisionBox* hit_box;												// 攻击碰撞箱
	StateMachine state_machine;											// 状态机
	Timer timer_invulnerable_status;									// 无敌状态计时器
	Timer timer_invulnerable_blink;										// 无敌时间闪烁计时器
	bool is_invulnerable_status = false;								// 是否无敌状态
	bool is_blink_invisible = false;									// 是否无敌闪烁的不可见状态
	AnimationGroup* current_animation = nullptr;						// 当前动画组
	std::unordered_map<std::string, AnimationGroup> animation_pool;		// 角色动画池

public:
	Character();
	~Character();

	void switch_state(const std::string& id);
	void make_invulnerable(bool is_blink = true, float delta_ratio = 1.0f);
	void set_animation(const std::string& id);
	void decrease_hp();

	virtual void reset() { hp = hp_max; }
	virtual void on_hurt() { };

	virtual void on_input(const ExMessage& msg) { };
	virtual void on_update(float delta);
	virtual void on_render();

	bool is_on_floor() const { return position.y >= FLOOR_Y; }
	float get_floor_y() const { return FLOOR_Y; }
	int get_hp_max() const { return hp_max; }
	int get_hp() const { return hp; }
	void set_gravity_enable(bool flag) { enable_gravity = flag; }
	void set_facing_left(bool flag) { is_facing_left = flag; }
	bool get_facing_left() const { return is_facing_left; }

	Vector2 get_logic_center() const { return { position.x, position.y - logic_height / 2 }; }
	void set_position(const Vector2& position) { this->position = position; }
	const Vector2& get_position() const { return position; }
	void set_velocity(const Vector2& velocity) { this->velocity = velocity; }
	const Vector2& get_velocity() const { return velocity; }

	CollisionBox* get_hurt_box() const { return hurt_box; }
	CollisionBox* get_hit_box() const { return hit_box; }
};