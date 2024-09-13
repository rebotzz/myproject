#pragma once
#include "character.h"

class EnemyDragonKing : public Character
{
public:
	enum class Direction
	{
		Up, Down, Left, Right
	};
	Direction attack_dir = Direction::Left;							// 攻击朝向

private:
	const float SPEED_JUMP_BIG = 1300.0f;
	const float SPEED_JUMP_SMALL = 880.0f;

private:
	// 状态标志
	bool is_attacking = false;
	bool is_rolling = false;
	bool is_big_jump = false;

	// 特效
	Animation animation_vfx_slash_up;								// 攻击刀光特效动画,上
	Animation animation_vfx_slash_down;								// 攻击刀光特效动画,下
	Animation animation_vfx_slash_left;								// 攻击刀光特效动画,左
	Animation animation_vfx_slash_right;							// 攻击刀光特效动画,右

	Animation* current_slash_animation = nullptr;					// 当前刀光动画
	CollisionBox* collision_box_katana = nullptr;					// 攻击刀光碰撞箱体

public:
	EnemyDragonKing();
	~EnemyDragonKing();

	virtual void on_input(const ExMessage& msg) override { };
	virtual void on_update(float delta) override;
	virtual void on_render() override;

	virtual void on_hurt() override;
	void on_attack();

	void set_rolling(bool flag) { is_rolling = flag; }
	bool get_rolling() const { return is_rolling; }
	void set_attacking(bool flag) { is_attacking = flag; }
	bool get_attacking() const { return is_attacking; }
	void set_big_jump(bool flag) { is_big_jump = flag; }
	bool get_big_jump() const { return is_big_jump; }
	float get_speed_jump_big() const { return SPEED_JUMP_BIG; }
	float get_speed_jump_small() const { return SPEED_JUMP_SMALL; }
	

	// 其他:可能有用,也可能没用
	void update_attack_box_position();
	void set_attack_dir(Direction dir) { attack_dir = dir; }

};