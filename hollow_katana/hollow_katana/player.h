#pragma once
#include "character.h"
#include "status_bar.h"

// �����
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

	const float BULLET_TIME_TOTAL = 2.0f;							// �ӵ�ʱ�����ֵ
	const float SPEED_DISPLACE_AXIS = 300.0f;						// ����λ��(ˮƽ)�ٶ�
	const float SPEED_DISPLACE_UP = 780.0f;						// ����λ��(��)�ٶ�

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
	bool is_dance_key_down = false;									// ������Ƿ���

	Animation animation_vfx_slash_up;								// ����������Ч����,��
	Animation animation_vfx_slash_down;								// ����������Ч����,��
	Animation animation_vfx_slash_left;								// ����������Ч����,��
	Animation animation_vfx_slash_right;							// ����������Ч����,��
	Direction attack_dir = Direction::Left;							// ��������
	Animation* current_slash_animation = nullptr;					// ��ǰ���⶯��

	Animation animation_vfx_jump;									// ��Ծ��Ч����
	Animation animation_vfx_land;									// �����Ч����
	bool is_vfx_jump_visiable = false;								// ��Ծ��Ч�����Ƿ�ɼ�
	bool is_vfx_land_visiable = false;								// �����Ч�����Ƿ�ɼ�

	Timer timer_bullet_time;										// �ӵ�ʱ�䶨ʱ��
	bool is_bullet_time_key_down = false;							// �Ƿ�����ӵ�ʱ��
	float current_bullet_time = 2.0f;								// ��ǰʣ����ӵ�ʱ��

	// ��ӹ�������Ķ�ʱ��
	Direction beat_displace_dir = Direction::Left;					// ����λ�Ʒ���
	bool is_displace_ex = false;									// �Ƿ�����λ��
	Timer timer_enable_displace_ex;
	Timer timer_displace_ex;										// ��ɫ����λ��(����)��ʱ��
	bool is_down_slash = false;										// �Ƿ�����
	bool is_sword_hit = false;										// �Ƿ�ƴ��
	Timer timer_delay_decrease_hp;									// ��ʱ��Ѫ���ڼ���Ƿ���ƴ��

	// �������к�����
	Timer timer_recoiling;											// ��������ʱ��
	float speed_jump = 780.0f;										// ��Ծ�߶ȿ���

	Timer timer_hit_effect;											// ����Ч����ʱ��
	bool is_hitting = false;										// ����Ч��״̬
	bool is_hit_cd_comp = true;										// ����Ч��CD

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
	void set_hp_max(int val) { hp_max = val; }


private:
	void on_hit_collide();
	// ���к�����
	void on_recoil(float delta = 0.0f);
	// ������������һ�ξ���
	void on_attack_displace_front();
	void create_hit_effect();
	void create_hurt_effect();
	void create_bullet_time_effect();
	void create_roll_effect();

private:
	void update_attack_dir(float mouse_x, float mouse_y);
	void update_attack_dir();
};