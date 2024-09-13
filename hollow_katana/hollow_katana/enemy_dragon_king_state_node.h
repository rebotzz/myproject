#pragma once
#include "state_node.h"
#include "timer.h"

// 状态节点:
// 1.闲置, 2.跳跃, 3.奔跑, 4.下落 
// 5.冲刺(地面冲刺,空中冲刺都用同一个逻辑处理,加上特效就行,本质还是身体撞人)
// 8.普通攻击(似乎可以加入拼刀机制,0.3s无敌帧[加一个半无敌],
// 然后取消闪烁特效,在进入无敌帧的接口处,加一个缺省值)
// 9.地面冲击波  
// 7.子弹(无论空中地面, 像hornet silk, 但是不取消重力)
// 10.三联招式
// 12.死亡
namespace EnemyDragonKing
{
	// [闲置]
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

	// [跳跃]
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

	// [奔跑]
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

	// [下落]
	class EnemyDragonKingFallState : public StateNode
	{
	public:
		EnemyDragonKingFallState() = default;
		~EnemyDragonKingFallState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};

	// [空中冲刺]
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

	// [地面冲刺]
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

	// [瞄准]空中冲刺前置状态
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

	// [下蹲]地面冲刺前置状态
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

	// [扔剑]
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

	// [扔刺球]
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

	// [扔丝线]
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

	// [死亡]
	class EnemyDragonKingDeadState : public StateNode
	{
	public:
		EnemyDragonKingDeadState() = default;
		~EnemyDragonKingDeadState() = default;
		virtual void on_enter()  override;
	};

}
