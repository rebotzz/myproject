#pragma once
#include <string>
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

namespace EnemyDragonKingState
{
	// [����]
	class IdleState : public StateNode
	{
	private:
		Timer timer;
		const float MIN_DIS = 350.f;

	public:
		IdleState();
		~IdleState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit() override;
	};

	// [��Ծ]
	class JumpState : public StateNode
	{
	private:
		const float SPEED_MOVE = 260.f;
		const float MIN_DIS = 400.0f;

	public:
		JumpState() = default;
		~JumpState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};

	// [����]
	class RunState : public StateNode
	{
	private:
		const float SPEED_RUN = 400.0f;
		const float MIN_DISTANCE = 350.f;
		std::string next_state;

	public:
		RunState() = default;
		~RunState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit()  override;
	};

	// [����]
	class FallState : public StateNode
	{
	public:
		FallState() = default;
		~FallState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};

	// [׼��]
	class PrepareState : public StateNode
	{
	private:
		Timer timer_effect;
		Timer timer_prepare;
		std::string next_state;

	public:
		PrepareState();
		~PrepareState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};


	// [��ͨһ��]
	class AttackNormalState : public StateNode
	{
	private:
		Timer timer_attack;
		Timer timer_duration;
		Timer timer_exit;
		const float SPEED_MOVE_AXIS = 500.f;
	public:
		AttackNormalState();
		~AttackNormalState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};



	// [����һ��]
	class ElectricState : public StateNode
	{
	private:
		Timer timer_duration;
		Timer timer_slow;
		Timer timer_effect_line;

	public:
		ElectricState();
		~ElectricState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit() override;
	};

	// [����嵯]
	class FireDashState : public StateNode
	{
	private:
		Timer timer_over;
		Timer timer_exit;
		Timer timer_max_time;
		Timer timer_min_time;
		bool min_can_finish = true;
		bool can_finish = false;

	public:
		FireDashState();
		~FireDashState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit() override;
	};

	// [���浯]
	class FireBulletState : public StateNode
	{
	public:
		FireBulletState() = default;
		~FireBulletState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit() override;
	};

	// [����]
	class DeadState : public StateNode
	{
		Timer timer;
		Timer timer_text1;
		Timer timer_text2;
	public:
		DeadState() = default;
		~DeadState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};
}
