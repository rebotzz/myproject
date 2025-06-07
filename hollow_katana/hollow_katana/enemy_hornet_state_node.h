#pragma once
#include "state_node.h"
#include "timer.h"

// ��Ʒ�״̬�ڵ�:
// 1.����, 2.��Ծ, 3.����, 4.���� 5.���г�� 6.������
// 7.��׼ 8.�ӽ�, 9.�Ӵ��� 10.��˿�� 11.�¶� 12.����

// ��Ʒ�AI�߼�״̬�ڵ� 2.0
// ����: 1.��Ѫ����[���ؽ�ս����] 2.��Ѫ����[����Զ�̹���] 

// [����]
class EnemyHornetIdleState : public StateNode
{
private:
	Timer timer;
	const float MIN_DIS = 300.0f;
	bool is_first_half_hp = true;

public:
	EnemyHornetIdleState();
	~EnemyHornetIdleState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit() override;
};

// [��Ծ]
class EnemyHornetJumpState : public StateNode
{
private:
	enum class NextState
	{
		Fall, Aim, ThrowSilk
	};
private:
	const float SPEED_JUMP = 1300.0f;
	const float SPEED_MOVE_AXIS = 300.0f;
	const float MIN_DIS = 400.0f;
	NextState next_state = NextState::Fall;

public:
	EnemyHornetJumpState() = default;
	~EnemyHornetJumpState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [����]
class EnemyHornetRunState : public StateNode
{
private:
	const float SPEED_RUN = 400.0f;
	const float MIN_DISTANCE = 250.0f;

public:
	EnemyHornetRunState() = default;
	~EnemyHornetRunState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [����]
class EnemyHornetFallState : public StateNode
{
public:
	EnemyHornetFallState() = default;
	~EnemyHornetFallState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [���г��]
class EnemyHornetDashInAirState : public StateNode
{
private:
	Timer timer;
	const float SPEED_DASH = 1500.0f;

public:
	EnemyHornetDashInAirState();
	~EnemyHornetDashInAirState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [������]
class EnemyHornetDashOnFloorState : public StateNode
{
private:
	Timer timer_dash;
	const float SPEED_DASH = 1500.0f;

public:
	EnemyHornetDashOnFloorState();
	~EnemyHornetDashOnFloorState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [��׼]���г��ǰ��״̬
class EnemyHornetAimState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetAimState();
	~EnemyHornetAimState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [�¶�]������ǰ��״̬
class EnemyHornetSquatState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetSquatState();
	~EnemyHornetSquatState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [�ӽ�]
class EnemyHornetThrowSwordState : public StateNode
{
private:
	Timer timer_throw;
	Timer timer_switch;

public:
	EnemyHornetThrowSwordState();
	~EnemyHornetThrowSwordState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [�Ӵ���]
class EnemyHornetThrowBarbsState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetThrowBarbsState();
	~EnemyHornetThrowBarbsState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [��˿��]
class EnemyHornetThrowSilkState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetThrowSilkState();
	~EnemyHornetThrowSilkState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [����]
class EnemyHornetDeadState : public StateNode
{
private:
	Timer timer_exit;
	Timer timer_dialogue;

public:
	EnemyHornetDeadState();
	~EnemyHornetDeadState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta);
};
