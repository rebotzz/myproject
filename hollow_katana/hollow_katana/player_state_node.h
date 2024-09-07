#pragma once
#include "state_node.h"
#include "player.h"

// Íæ¼ÒµÄËùÓÐ×´Ì¬½Úµã:ÏÐÖÃ,±¼ÅÜ,¹¥»÷,ÌøÔ¾,ÏÂÂä,·­¹ö,ËÀÍö
class PlayerAttackState : public StateNode
{
private:
	Timer timer;

public:
	PlayerAttackState();
	~PlayerAttackState();
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
	void update_hit_box_position();
};

class PlayerIdleState : public StateNode
{
public:
	PlayerIdleState();
	~PlayerIdleState();
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

class PlayerRunState : public StateNode
{
public:
	PlayerRunState();
	~PlayerRunState();
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
	~PlayerRollState();
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

class PlayerJumpState : public StateNode
{
private:
	Timer timer;

public:
	PlayerJumpState();
	~PlayerJumpState();
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

class PlayerFallState : public StateNode
{
private:
	Timer timer;

public:
	PlayerFallState();
	~PlayerFallState();
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

class PlayerDeadState : public StateNode
{
private:
	Timer timer;

public:
	PlayerDeadState();
	~PlayerDeadState();
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};