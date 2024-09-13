#pragma once
#include "character.h"

class EnemyDragonKing : public Character
{
public:
	enum class Direction
	{
		Up, Down, Left, Right
	};
	Direction attack_dir = Direction::Left;							// ��������

private:
	const float SPEED_JUMP_BIG = 1300.0f;
	const float SPEED_JUMP_SMALL = 880.0f;

private:
	// ״̬��־
	bool is_attacking = false;
	bool is_rolling = false;
	bool is_big_jump = false;

	// ��Ч
	Animation animation_vfx_slash_up;								// ����������Ч����,��
	Animation animation_vfx_slash_down;								// ����������Ч����,��
	Animation animation_vfx_slash_left;								// ����������Ч����,��
	Animation animation_vfx_slash_right;							// ����������Ч����,��

	Animation* current_slash_animation = nullptr;					// ��ǰ���⶯��
	CollisionBox* collision_box_katana = nullptr;					// ����������ײ����

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
	

	// ����:��������,Ҳ����û��
	void update_attack_box_position();
	void set_attack_dir(Direction dir) { attack_dir = dir; }

};