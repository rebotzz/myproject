#pragma once
#include "state_node.h"
#include "timer.h"

// ״̬�ڵ�:
// 1.����, 2.��Ծ, 3.����, 4.���� 
// 5.���(������,���г�̶���ͬһ���߼�����,������Ч����,���ʻ�������ײ��)
// 8.��ͨ����(�ƺ����Լ���ƴ������,0.3s�޵�֡[��һ�����޵�],
// Ȼ��ȡ����˸��Ч,�ڽ����޵�֡�Ľӿڴ�,��һ��ȱʡֵ)
// 9.��������  
// 7.�ӵ�(���ۿ��е���, ��hornet silk, ���ǲ�ȡ������)
// 10.������ʽ
// 12.����
namespace EnemyDragonKing
{
	// [����]
	class EnemyDragonKingIdleState : public StateNode
	{
	private:
		Timer timer;

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
		bool is_jump_left = false;
		const float SPEED_JUMP = 1300.0f;

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

	// [���г��]
	class EnemyDragonKingDashInAirState : public StateNode
	{
	private:
		Timer timer;
		const float SPEED_DASH = 1500.0f;

	public:
		EnemyDragonKingDashInAirState();
		~EnemyDragonKingDashInAirState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit()  override;
	};

	// [������]
	class EnemyDragonKingOnDashFloorState : public StateNode
	{
	private:
		Timer timer;
		const float SPEED_DASH = 1500.0f;

	public:
		EnemyDragonKingOnDashFloorState();
		~EnemyDragonKingOnDashFloorState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit()  override;
	};

	// [��׼]���г��ǰ��״̬
	class EnemyDragonKingAimState : public StateNode
	{
	private:
		Timer timer;

	public:
		EnemyDragonKingAimState();
		~EnemyDragonKingAimState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};

	// [�¶�]������ǰ��״̬
	class EnemyDragonKingSquatState : public StateNode
	{
	private:
		Timer timer;

	public:
		EnemyDragonKingSquatState();
		~EnemyDragonKingSquatState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};

	// [�ӽ�]
	class EnemyDragonKingThrowSwordState : public StateNode
	{
	private:
		Timer timer_throw;
		Timer timer_switch;

	public:
		EnemyDragonKingThrowSwordState();
		~EnemyDragonKingThrowSwordState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};

	// [�Ӵ���]
	class EnemyDragonKingThrowBarbsState : public StateNode
	{
	private:
		Timer timer;

	public:
		EnemyDragonKingThrowBarbsState();
		~EnemyDragonKingThrowBarbsState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit()  override;
	};

	// [��˿��]
	class EnemyDragonKingThrowSilkState : public StateNode
	{
	private:
		Timer timer;

	public:
		EnemyDragonKingThrowSilkState();
		~EnemyDragonKingThrowSilkState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};

	// [����]
	class EnemyDragonKingDeadState : public StateNode
	{
	public:
		EnemyDragonKingDeadState() = default;
		~EnemyDragonKingDeadState() = default;
		virtual void on_enter()  override;
	};

}
