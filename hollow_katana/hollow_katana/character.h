#pragma once
#include <easyx.h>
#include "vector2.h"
#include "timer.h"
#include "animation.h"
#include "collision_manager.h"
#include "state_machine.h"

// ��ɫ����
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
																		
protected:																
	int hp = 10;														// ����ֵ
	float logic_height = 0.0f;											// �߼��߶�
	bool is_face_left = false;											// ��ɫ����
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
	AnimationGroup* current_animation = nullptr;						// ��ǰ������
	std::unordered_map<std::string, AnimationGroup> animation_pool;		// ��ɫ������

public:
	Character();
	~Character();

	void switch_state(const std::string& id);
	void make_invulnerable();
	void set_animation(const std::string& id);
	void decrease_hp();

	virtual void on_hurt() { };

	virtual void on_input(const ExMessage& msg) { };
	virtual void on_update(float delta);
	virtual void on_render();

	int get_hp() const { return hp; }
	void set_gravity_enable(bool flag) { enable_gravity = flag; }
	bool is_on_floor() const { return position.y >= FLOOR_Y; }
	float get_floor_y() const { return FLOOR_Y; }

	Vector2 get_logic_center() const { return { position.x, position.y - logic_height / 2 }; }
	void set_position(const Vector2& position) { this->position = position; }
	const Vector2& get_position() const { return position; }
	void set_velocity(const Vector2& velocity) { this->velocity = velocity; }
	const Vector2& get_velocity() const { return velocity; }

	CollisionBox* get_hurt_box() const { return hurt_box; }
	CollisionBox* get_hit_box() const { return hit_box; }
};