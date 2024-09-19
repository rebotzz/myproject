#pragma once
#include <easyx.h>
#include "vector2.h"
#include "timer.h"
#include "animation.h"
#include "collision_manager.h"
#include "state_machine.h"

// ��ɫ����, ��ɫ��״̬������
class Character
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
	const float TIME_INVULNERABLE = 1.0f;								// �޵�֡ʱ��

protected:						
	int hp_max = 1;														// ����ֵ����
	int hp = 1;															// ����ֵ
	float logic_height = 0.0f;											// �߼��߶�
	bool is_facing_left = false;										// ��ɫ����
	bool enable_gravity = true;											// �Ƿ�ʼ����
	Vector2 position;													// ��ɫλ��,�ײ�����ê��ģʽ
	Vector2 velocity;													// �ٶ�
	CollisionBox* hurt_box;												// �ܻ���ײ��
	CollisionBox* hit_box;												// ������ײ��
	StateMachine state_machine;											// ״̬��
	Timer timer_invulnerable_status;									// �޵�״̬��ʱ��
	Timer timer_invulnerable_blink;										// �޵�ʱ����˸��ʱ��
	bool is_invulnerable_status = false;								// �Ƿ��޵�״̬
	bool is_blink_invisible = false;									// �Ƿ��޵���˸�Ĳ��ɼ�״̬
	bool not_blink = false;												// ����˸
	bool is_invisible = false;											// �Ƿ�ɼ�,������Ч,���ر���
	AnimationGroup* current_animation = nullptr;						// ��ǰ������
	std::unordered_map<std::string, AnimationGroup> animation_pool;		// ��ɫ������

public:
	Character();
	~Character();

	void switch_state(const std::string& id);
	void make_invulnerable(bool not_blink_ = false, float delta_ratio = 1.0f);
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
	float get_gravity() const { return GRAVITY; }
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

	// ʱ�����2.0�ع��ӿ�
	HistoryStatus get_current_status() const 
	{ 
		return (is_facing_left ? current_animation->left : current_animation->right).get_current_status();
	}
};