#pragma once
#include <string>
#include "state_node.h"
#include "timer.h"


// [龙王]状态节点:
// 策略: 半血以上[攻击频率低,没有大招] 半血以下[攻击频率高,大招] (加入拼刀机制,0.45s无敌帧)
// 1.闲置, 2.跳跃, 3.奔跑, 4.下落 
// 5.准备 6.普通攻击 7.雷闪一刀 8.火焰冲弹 
// 9.空中火焰弹 10.死亡
// 
// 废弃方案:
// 1.三联招式+闪现 2.地面冲击波 3.天降剑雨


namespace EnemyDragonKingState
{
	// [闲置]
	class IdleState : public StateNode
	{
	private:
		Timer timer;
		const float MIN_DIS = 350.f;
		bool is_first_half_hp = true;

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
		const float SPEED_MOVE = 420.f;
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
		bool is_start = false;
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

	// [火焰弹]空中
	class FireBulletState : public StateNode
	{
	private:

	public:
		FireBulletState();
		~FireBulletState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
		virtual void on_exit() override;
	};

	// [死亡]
	class DeadState : public StateNode
	{
		Timer timer_text1;
		Timer timer_text2;
	public:
		DeadState();
		~DeadState() = default;
		virtual void on_enter()  override;
		virtual void on_update(float delta) override;
	};
}
