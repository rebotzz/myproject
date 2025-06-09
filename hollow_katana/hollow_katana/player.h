#pragma once
#include "character.h"
#include "status_bar.h"


// �����
class Player : public Character
{
	friend class PlayerControlAWSD;
	friend class PlayerControlArrow;

private:
	const float SPEED_ROLL = 800.0f;								// �����ٶ�
	const float ROLL_CD = 0.95f;									// ����CD
	const float ATTACK_CD = 0.7f;									// ����CD
	const float BULLET_TIME_TOTAL = 2.0f;							// �ӵ�ʱ�����ֵ
	const float SPEED_DISPLACE_AXIS = 300.0f;						// ����λ��(ˮƽ)�ٶ�
	const float SPEED_DISPLACE_UP = 780.0f;							// ����λ��(��)�ٶ�

private:
	Timer timer_roll_cd;											// ������ȴʱ�䶨ʱ��
	bool is_rolling = false;										// �Ƿ����ڷ���
	bool is_roll_cd_comp = true;									// ������ȴʱ���Ƿ����

	Animation animation_vfx_slash_up;								// ����������Ч����,��
	Animation animation_vfx_slash_down;								// ����������Ч����,��
	Animation animation_vfx_slash_left;								// ����������Ч����,��
	Animation animation_vfx_slash_right;							// ����������Ч����,��
	Direction attack_dir = Direction::Left;							// ��������
	Animation* current_slash_animation = nullptr;					// ��ǰ���⶯��

	Timer timer_bullet_time;										// �ӵ�ʱ�䶨ʱ��
	bool is_bullet_time_key_down = false;							// �Ƿ�����ӵ�ʱ��
	float current_bullet_time = 2.0f;								// ��ǰʣ����ӵ�ʱ��

	// ��ӹ�������Ķ�ʱ��
	Direction beat_displace_dir = Direction::Left;					// ����λ�Ʒ���
	bool is_displace_ex = false;									// �Ƿ�����λ��
	Timer timer_enable_displace_ex;									// ����λ�Ƴ���ʱ�䶨ʱ��
	Timer timer_displace_ex;										// ��ɫ����λ��(����)λ���ۼ��ƶ���ʱ��
	bool is_down_slash = false;										// �Ƿ�����
	bool is_sword_hit = false;										// �Ƿ�ƴ��
	Timer timer_delay_decrease_hp;									// ��ʱ��Ѫ���ڼ���Ƿ���ƴ��
	Timer timer_recoiling;											// �������к�������ʱ��
	Timer timer_hit_effect;											// ����Ч����ʱ��
	bool is_hitting = false;										// ����Ч��״̬
	bool is_hit_eff_cd_comp = true;									// ����Ч��CD
	Timer timer_create_particle_effect;								// ������Ч������
	Vector2 pos_cursor = { 0, 0 };									// ʮ����׼ͼ��(���)λ��
	IMAGE* img_crosshair = nullptr;									// ʮ����׼ͼ��

	StatusBar status_bar;											// ״̬��
	bool is_control_preset_awsd = true;								// �Ƿ����ð���Ԥ��1:AWSD����

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

	// get��set����
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

	// ����Ԥ���л�
	void set_control_preset(bool flag) { is_control_preset_awsd = flag; }
	bool get_control_preset() { return is_control_preset_awsd; }
	void set_bullet_time_keydown(bool flag) { is_bullet_time_key_down = flag; }
	virtual bool can_skill_1() const override { return can_roll(); }
	virtual bool can_skill_2() const override { return can_dance(); }
	virtual void release_skill_1() { switch_state("roll"); }
	virtual void release_skill_2() { switch_state("dance"); }

private:
	void on_hit_collide();					// �������лص��߼�
	void on_recoil(float delta = 0.0f);		// ���к�����
	void on_attack_direction_move();		// ������������һ�ξ���
	void create_hit_effect();				// ������Ч
	void create_hurt_effect();				// �ܻ���Ч
	void create_bullet_time_effect();		// �ӵ�ʱ���Ӱ��Ч
	void create_roll_effect();				// ������Ч
	void update_attack_dir();
	void update_attack_dir(float mouse_x, float mouse_y);
};