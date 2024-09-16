#pragma once
#include <memory>
#include "character.h"
#include "fire_bullet.h"

class EnemyDragonKing : public Character
{
public:
	enum class Direction
	{
		Up, Down, Left, Right
	};
	Direction attack_dir = Direction::Left;							// 攻击朝向

private:
	const float SPEED_JUMP = 1300.0f;
	const float SPEED_JUMP_SMALL = 880.0f;

private:
	// 状态标志
	bool is_attacking = false;
	bool is_rolling = false;
	bool is_fire_dash = false;

	// 特效
	Animation animation_vfx_slash_up;								// 攻击刀光特效动画,上
	Animation animation_vfx_slash_down;								// 攻击刀光特效动画,下
	Animation animation_vfx_slash_left;								// 攻击刀光特效动画,左
	Animation animation_vfx_slash_right;							// 攻击刀光特效动画,右
	Animation* current_slash_animation = nullptr;					// 当前刀光动画

	Animation animation_fire_dash_left;								// 火焰冲弹
	Animation animation_fire_dash_right;							// 火焰冲弹
	Animation* current_fire_dash_animation = nullptr;				// 当前火焰冲弹动画

	CollisionBox* collision_box_katana = nullptr;					// 攻击刀光碰撞箱体
	CollisionBox* collision_box_fire_dash = nullptr;				// 火焰冲刺碰撞箱体

	// 远程攻击
	Vector2 position_fire_bullet;
	std::vector <std::shared_ptr<FireBullet>> fire_bullet_list;

public:
	EnemyDragonKing();
	~EnemyDragonKing();

	virtual void on_input(const ExMessage& msg) override { };
	virtual void on_update(float delta) override;
	virtual void on_render() override;

	virtual void on_hurt() override;
	void on_attack();
	void on_fire_dash();
	void on_fire_bullet();

	void set_rolling(bool flag) { is_rolling = flag; }
	bool get_rolling() const { return is_rolling; }
	void set_attacking(bool flag) { is_attacking = flag; }
	bool get_attacking() const { return is_attacking; }
	float get_speed_jump() const { return SPEED_JUMP; }
	
	// 其他
	void update_attack_box_position();
	void set_attack_dir(Direction dir) { attack_dir = dir; }
	void set_fire_dash(bool flag) { is_fire_dash = flag; }
};