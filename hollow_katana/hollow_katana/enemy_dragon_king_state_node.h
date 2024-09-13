#pragma once
#include "state_node.h"
#include "timer.h"

// ���� ״̬�ڵ�:
// ����: 1.��Ѫ����[����Ƶ�ʵ�,û�д���] 2.��Ѫ����[����Ƶ�ʸ�,����] 
// 
// 1.����, 2.��Ծ, 3.����, 4.���� 
// 5.���(������,���г�̶���ͬһ���߼�����,������Ч����,���ʻ�������ײ��)
// 8.��ͨ����(�ƺ����Լ���ƴ������,0.3s�޵�֡[��һ�����޵�],
// Ȼ��ȡ����˸��Ч,�ڽ����޵�֡�Ľӿڴ�,��һ��ȱʡֵ)
// 9.��������  
// 7.�ӵ�(���ۿ��е���, ��hornet silk, ���ǲ�ȡ������)
// 10.������ʽ ����
// 12.����

// [����]
class EnemyDragonKingIdleState : public StateNode
{
private:
	Timer timer;
	const float MIN_DIS = 300.f;

public:
	EnemyDragonKingIdleState();
	~EnemyDragonKingIdleState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit() override;
};

// [��Ծ]
class EnemyDragonKingJumpState : public StateNode
{
private:
	const float SPEED_MOVE_SLOW = 150.f;
	const float SPEED_MOVE_FAST = 430.f;
	const float MIN_DIS = 400.0f;

public:
	EnemyDragonKingJumpState() = default;
	~EnemyDragonKingJumpState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [����]
class EnemyDragonKingRunState : public StateNode
{
private:
	const float SPEED_RUN = 400.0f;
	const float MIN_DISTANCE = 250.0f;

public:
	EnemyDragonKingRunState() = default;
	~EnemyDragonKingRunState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [����]
class EnemyDragonKingFallState : public StateNode
{
public:
	EnemyDragonKingFallState() = default;
	~EnemyDragonKingFallState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [�չ�]
class EnemyDragonKingAttackState : public StateNode
{
private:
	Timer timer_attack;
	Timer timer_attack_duration;
	Timer timer_exit;
	const float SPEED_MOVE_AXIS = 500.f;
public:
	EnemyDragonKingAttackState();
	~EnemyDragonKingAttackState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};