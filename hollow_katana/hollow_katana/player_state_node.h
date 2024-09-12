#pragma once
#include "state_node.h"
#include "timer.h"

// 玩家的所有状态节点:闲置,奔跑,攻击,跳跃,下落,翻滚,死亡

// 攻击/翻滚的定时器可以去掉,功能由动画结束的回调完成
class PlayerAttackState : public StateNode
{
public:
	PlayerAttackState() = default;
	~PlayerAttackState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

class PlayerRollState : public StateNode
{
public:
	PlayerRollState() = default;
	~PlayerRollState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

class PlayerIdleState : public StateNode
{
public:
	PlayerIdleState() = default;
	~PlayerIdleState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

class PlayerRunState : public StateNode
{
public:
	PlayerRunState() = default;
	~PlayerRunState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

class PlayerJumpState : public StateNode
{
public:
	PlayerJumpState() = default;
	~PlayerJumpState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

class PlayerFallState : public StateNode
{
public:
	PlayerFallState() = default;
	~PlayerFallState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

class PlayerDeadState : public StateNode
{
private:
	Timer timer;
	bool can_next = false;

public:
	PlayerDeadState();
	~PlayerDeadState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};


class PlayerDanceState : public StateNode
{
public:
	PlayerDanceState() = default;
	~PlayerDanceState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};


