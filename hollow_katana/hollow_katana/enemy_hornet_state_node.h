#pragma once
#include "state_node.h"
#include "timer.h"

// ��Ʒ�״̬�ڵ�:
// 1.����, 2.��Ծ, 3.����, 4.���� 5.���г�� 6.������
// 7.��׼ 8.�ӽ�, 9.�Ӵ��� 10.��˿�� 11.����

// [����]����ת״̬: ���� ��Ծ ���� ��׼ �ӽ� �Ӵ��� ��˿��
class EnemyHornetIdleState : public StateNode
{
private:
	Timer timer;
	const float CD = 1.0f;
	bool can_switch_state = false;

public:
	EnemyHornetIdleState();
	~EnemyHornetIdleState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};


// [��Ծ]����ת״̬: ���� ���� ���г�� ��˿��  ->���ƶ��ӽ����
class EnemyHornetJumpState : public StateNode
{
private:
	bool is_jump_left = false;

public:
	EnemyHornetJumpState() = default;
	~EnemyHornetJumpState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [����]����ת״̬: ���� ���� ��Ծ ��׼ ��˿��	->���ƶ��ӽ����
class EnemyHornetRunState : public StateNode
{
public:
	EnemyHornetRunState() = default;
	~EnemyHornetRunState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [����]����ת״̬: ���� ���� 	->���ƶ��ӽ����
class EnemyHornetFallState : public StateNode
{
public:
	EnemyHornetFallState() = default;
	~EnemyHornetFallState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [���г��]����ת״̬: ���� ����
class EnemyHornetDashAirState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetDashAirState();
	~EnemyHornetDashAirState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [������]����ת״̬: ���� ����
class EnemyHornetDashFloorState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetDashFloorState();
	~EnemyHornetDashFloorState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [��׼]����ת״̬: ���� ������
class EnemyHornetAimState : public StateNode
{
private:
	Timer timer;
	const float CD = 0.6f;

public:
	EnemyHornetAimState();
	~EnemyHornetAimState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};


// [�ӽ�]����ת״̬: ���� ����
class EnemyHornetThrowSwordState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetThrowSwordState();
	~EnemyHornetThrowSwordState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [�Ӵ���]����ת״̬: ���� ����
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

// [��˿��]����ת״̬: ���� ����
class EnemyHornetThrowSilkState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetThrowSilkState();
	~EnemyHornetThrowSilkState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [����]: ����ս��
class EnemyHornetDeadState : public StateNode
{
public:
	EnemyHornetDeadState() = default;
	~EnemyHornetDeadState() = default;
	virtual void on_enter()  override;
};
