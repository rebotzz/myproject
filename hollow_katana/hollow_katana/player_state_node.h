#pragma once
#include "state_node.h"
#include "timer.h"

// ��ҵ�����״̬�ڵ�:����,����,����,��Ծ,����,����,����

// ����/�����Ķ�ʱ������ȥ��,�����ɶ��������Ļص����
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
	void update_hit_box_position();
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

public:
	PlayerDeadState();
	~PlayerDeadState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};