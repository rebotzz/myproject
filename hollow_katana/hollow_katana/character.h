#pragma once
//#include <easyx.h>
#include <graphics.h>		//easyxͼ�ο�
#include "vector2.h"
#include "timer.h"
#include "animation.h"
#include "collision_manager.h"
#include "state_machine.h"
#include "control.h"
#include "game_object.h"

#include <iostream>


// ��ɫ����, ��ɫ��״̬������
class Character : public GameObject
{
protected:
	struct AnimationGroup
	{
		Animation left;
		Animation right;
	};

protected:																
	const float FLOOR_Y = 620;											// ������ֱ�߶�y����
	const float GRAVITY = 980 * 2;										// ��������
	const float TIME_INVINCIBLE = 1.0f;									// �޵�֡ʱ��
	const float SPEED_JUMP_MAX = 780.0f;
	float speed_jump = 780.0f;											// ��Ծ�߶ȿ���
	const float SPEED_RUN = 300.0f;

protected:						
	int hp_max = 1;														// ����ֵ����
	int hp = 1;															// ����ֵ
	float logic_height = 0.0f;											// �߼��߶�
	bool is_facing_left = false;										// ��ɫ����
	bool enable_gravity = true;											// �Ƿ�ʼ����
	Vector2 position;													// ��ɫλ��,�ײ�����ê��ģʽ
	Vector2 velocity;													// �ٶ�
	CollisionBox* hurt_box = nullptr;									// �ܻ���ײ��
	CollisionBox* hit_box = nullptr;									// ������ײ��
	CollisionBox* interact_box = nullptr;								// ������ײ��
	StateMachine* state_machine = nullptr;								// ״̬��
	Timer timer_invincible_status;										// �޵�״̬��ʱ��
	Timer timer_invincible_blink;										// �޵�ʱ����˸��ʱ��
	bool is_invincible_status = false;									// �Ƿ��޵�״̬
	bool is_blink_invisible = false;									// �Ƿ��޵���˸�Ĳ��ɼ�״̬
	bool not_blink = false;												// ����˸
	bool is_invisible = false;											// �Ƿ�ɼ�,������Ч,���ر���
	AnimationGroup* current_animation = nullptr;						// ��ǰ������
	std::unordered_map<std::string, AnimationGroup> animation_pool;		// ��ɫ������

	Timer timer_attack_cd;												// ������ȴʱ�䶨ʱ��
	bool is_attacking = false;											// �Ƿ����ڹ���
	bool is_attack_cd_comp = true;										// ������ȴʱ���Ƿ����

	Animation animation_vfx_jump;										// ��Ծ��Ч����
	Animation animation_vfx_land;										// �����Ч����
	bool is_vfx_jump_visiable = false;									// ��Ծ��Ч�����Ƿ�ɼ�
	bool is_vfx_land_visiable = false;									// �����Ч�����Ƿ�ɼ�

	// ��Ծƽ̨
	// ƽ̨����߶�,��Ϊĳһʱ��ֻ�����һ��ƽ̨��;��ô����,��ɫ�����Լ�����ƽ̨���Լ���Ծ����ÿ����ɫӵ�ж���ƽ̨����߶�
	float platform_floor_y = 720.0f;	
	Timer timer_platform_reset;											// ƽ̨����߶Ȼָ���ʱ��
	float prev_frame_pos_y = 0.0f;										// ��һ֡��ɫY������λ��

	// ��ɫ����
	bool is_left_key_down = false;										// �����ƶ����Ƿ���
	bool is_right_key_down = false;										// �����ƶ����Ƿ���
	bool is_jump_key_down = false;										// ��Ծ���Ƿ���
	bool is_dash_key_down = false;										// ���/�������Ƿ���
	bool is_attack_key_down = false;									// �������Ƿ���
	bool is_skill_1_key_down = false;									// 1���ܼ��Ƿ���
	bool is_skill_2_key_down = false;									// 2���ܼ��Ƿ���
	Control* control = nullptr;											// ��ɫ�������

	// TODO:�����ЧID���������ô״̬�ڵ㲥����Ч���Ը�ͨ��
	std::string audio_run;


public:
	Character();
	~Character();

	// ���Ľӿ�
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

	// ��ɫ����
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

	// �����ͷ�
	virtual bool can_skill_1() const { return is_skill_1_key_down; }
	virtual bool can_skill_2() const { return is_skill_2_key_down; }
	virtual void release_skill_1() {}
	virtual void release_skill_2() {}
	virtual void release_attack() {}

	// ʱ�����2.0�ӿ�
	HistoryStatus get_current_status() const 
	{ 
		return (is_facing_left ? current_animation->left : current_animation->right).get_current_status();
	}

	void on_platform(bool flag) { if (flag) timer_platform_reset.restart(); }

	// Ĭ��ȱʡ������������ƽ̨����߶�;���ûվ��ƽ̨�ϣ�һ��ʱ���ȡ��ƽ̨
	void set_platform_floor_y(float val = 720.0f) { platform_floor_y = val; }
	bool is_on_floor() const { return position.y >= FLOOR_Y || position.y >= platform_floor_y; }
	float get_floor_y() const { return min(FLOOR_Y, platform_floor_y); }
	float get_prev_frame_pos_y() const { return prev_frame_pos_y; }
};