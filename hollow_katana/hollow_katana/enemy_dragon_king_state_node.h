#pragma once
#include <string>
#include "state_node.h"
#include "timer.h"


// 龙王 状态节点:
// 策略: 1.半血以上[攻击频率低,没有大招] 2.半血以下[攻击频率高,大招] 
// 
// 1.闲置, 2.跳跃, 3.奔跑, 4.下落 
// 5.冲刺(地面冲刺,空中冲刺都用同一个逻辑处理,加上特效就行,本质还是身体撞人)
// 8.普通攻击(似乎可以加入拼刀机制,0.3s无敌帧[加一个半无敌],
// 然后取消闪烁特效,在进入无敌帧的接口处,加一个缺省值)
// 9.地面冲击波  
// 7.子弹(无论空中地面, 像hornet silk, 但是不取消重力)
// 10.三联招式 闪现
// 12.死亡

namespace EnemyDragonKingState
{
	// [闲置]
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

	// [跳跃]
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

	// [奔跑]
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

	// [下落]
	class FallState : public StateNode
	{
	public:
		FallState() = default;
		~FallState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};

	// [准备]
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


	// [普通一刀]
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



	// [雷闪一刀]
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

	// [火焰冲弹]
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

	// [火焰弹]
	class FireBulletState : public StateNode
	{
	public:
		FireBulletState() = default;
		~FireBulletState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit() override;
	};

	// [死亡]
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
