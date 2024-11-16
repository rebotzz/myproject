#pragma once
#include "state_node.h"
#include "timer.h"

// 玩家的所有状态节点:闲置,奔跑,攻击,跳跃,下落,翻滚,死亡

// 攻击/翻滚的定时器可以去掉,功能由动画结束的回调完成
// bug修复: [攻击/翻滚]依靠动画结束取消[攻击/翻滚]状态有时候会出错,动画停在最后一帧不结束,比如突然状态跳转,动画切换
// 但是，在状态切出是取消状态标记位就行，似乎也可以不用定时器
class PlayerAttackState : public StateNode
{
private:
	Timer timer;			
public:
	PlayerAttackState();
	~PlayerAttackState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

class PlayerRollState : public StateNode
{
private:
	Timer timer;
public:
	PlayerRollState();
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
	virtual void on_exit()  override;
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

class PlayerDanceState : public StateNode
{
private:
	Timer timer;		// 跳舞的隐藏特性：回血
public:
	PlayerDanceState();
	~PlayerDanceState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

class PlayerDeadState : public StateNode
{
private:

public:
	PlayerDeadState();
	~PlayerDeadState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};
