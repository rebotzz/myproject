#pragma once
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

// [闲置]
class EnemyDragonKingIdleState : public StateNode
{
private:
	Timer timer;
	const float MIN_DIS = 400.0f;

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
	const float SPEED_MOVE_AXIS = 300.0f;
	const float MIN_DIS = 400.0f;

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
