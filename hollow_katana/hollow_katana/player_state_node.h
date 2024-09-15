#pragma once
#include "state_node.h"
#include "timer.h"

// ��ҵ�����״̬�ڵ�:����,����,����,��Ծ,����,����,����

// ����/�����Ķ�ʱ������ȥ��,�����ɶ��������Ļص����
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

class PlayerDanceState : public StateNode
{
public:
	PlayerDanceState() = default;
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
