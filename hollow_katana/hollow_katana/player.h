#pragma once
#include "character.h"
#include "status_bar.h"

// �����
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
	Timer timer_roll_cd;											// ������ȴʱ�䶨ʱ��
	bool is_rolling = false;										// �Ƿ����ڷ���
	bool is_roll_cd_comp = true;									// ������ȴʱ���Ƿ����

	Timer timer_attack_cd;											// ������ȴʱ�䶨ʱ��
	bool is_attacking = false;										// �Ƿ����ڹ���
	bool is_attack_cd_comp = true;									// ������ȴʱ���Ƿ����

	bool is_left_key_down = false;									// �����ƶ����Ƿ���
	bool is_right_key_down = false;									// �����ƶ����Ƿ���
	bool is_jump_key_down = false;									// ��Ծ���Ƿ���
	bool is_roll_key_down = false;									// �������Ƿ���
	bool is_attack_key_down = false;								// �������Ƿ���

	Animation animation_vfx_slash_up;								// ����������Ч����,��
	Animation animation_vfx_slash_down;								// ����������Ч����,��
	Animation animation_vfx_slash_left;								// ����������Ч����,��
	Animation animation_vfx_slash_right;							// ����������Ч����,��
	AttackDir attack_dir = AttackDir::Left;							// ��������
	Animation* current_slash_animation = nullptr;					// ��ǰ���⶯��

	Animation animation_vfx_jump;									// ��Ծ��Ч����
	Animation animation_vfx_land;									// �����Ч����
	bool is_vfx_jump_visiable = false;								// ��Ծ��Ч�����Ƿ�ɼ�
	bool is_vfx_land_visiable = false;								// �����Ч�����Ƿ�ɼ�

	Timer timer_bullet_time;										// �ӵ�ʱ�䶨ʱ��
	bool is_bullet_time = false;									// �Ƿ�����ӵ�ʱ��
	float current_bullet_time = 2.0f;								// ��ǰʣ����ӵ�ʱ��

	Timer timer_beating_displace;									// ��ɫ����λ��(��Ϊ�����λ��)��ʱ��
	bool is_beating_displace = false;								// �Ƿ�����λ��
	Timer timer_enable_beat_displace;
	float beating_displace_time = 0.3f;								// ����λ�Ƴ���ʱ��

	BeatDisplaceDir beat_displace_dir = BeatDisplaceDir::Left;		// ����λ�Ʒ���

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