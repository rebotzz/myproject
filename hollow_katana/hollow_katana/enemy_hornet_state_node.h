#pragma once
#include "state_node.h"
#include "timer.h"

// ´ó»Æ·ä×´Ì¬½Úµã:
// 1.ÏĞÖÃ, 2.ÌøÔ¾, 3.±¼ÅÜ, 4.ÏÂÂä 5.¿ÕÖĞ³å´Ì 6.µØÃæ³å´Ì
// 7.Ãé×¼ 8.ÈÓ½£, 9.ÈÓ´ÌÇò 10.ÈÓË¿Ïß 11.ÏÂ¶× 12.ËÀÍö

// ´ó»Æ·äAIÂß¼­×´Ì¬½Úµã 2.0
// ²ßÂÔ: 1.°ëÑªÒÔÉÏ[²àÖØ½üÕ½¹¥»÷] 2.°ëÑªÒÔÏÂ[²àÖØÔ¶³Ì¹¥»÷] 

// [ÏĞÖÃ]
class EnemyHornetIdleState : public StateNode
{
private:
	Timer timer;
	const float MIN_DIS = 300.0f;
	bool is_first_half_hp = true;

public:
	EnemyHornetIdleState();
	~EnemyHornetIdleState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit() override;
};

// [ÌøÔ¾]
class EnemyHornetJumpState : public StateNode
{
private:
	enum class NextState
	{
		Fall, Aim, ThrowSilk
	};
private:
	const float SPEED_JUMP = 1300.0f;
	const float SPEED_MOVE_AXIS = 300.0f;
	const float MIN_DIS = 400.0f;
	NextState next_state = NextState::Fall;

public:
	EnemyHornetJumpState() = default;
	~EnemyHornetJumpState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [±¼ÅÜ]
class EnemyHornetRunState : public StateNode
{
private:
	const float SPEED_RUN = 400.0f;
	const float MIN_DISTANCE = 250.0f;

public:
	EnemyHornetRunState() = default;
	~EnemyHornetRunState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [ÏÂÂä]
class EnemyHornetFallState : public StateNode
{
public:
	EnemyHornetFallState() = default;
	~EnemyHornetFallState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [¿ÕÖĞ³å´Ì]
class EnemyHornetDashInAirState : public StateNode
{
private:
	Timer timer;
	const float SPEED_DASH = 1500.0f;

public:
	EnemyHornetDashInAirState();
	~EnemyHornetDashInAirState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [µØÃæ³å´Ì]
class EnemyHornetDashOnFloorState : public StateNode
{
private:
	Timer timer_dash;
	const float SPEED_DASH = 1500.0f;

public:
	EnemyHornetDashOnFloorState();
	~EnemyHornetDashOnFloorState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [Ãé×¼]¿ÕÖĞ³å´ÌÇ°ÖÃ×´Ì¬
class EnemyHornetAimState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetAimState();
	~EnemyHornetAimState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [ÏÂ¶×]µØÃæ³å´ÌÇ°ÖÃ×´Ì¬
class EnemyHornetSquatState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetSquatState();
	~EnemyHornetSquatState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [ÈÓ½£]
class EnemyHornetThrowSwordState : public StateNode
{
private:
	Timer timer_throw;
	Timer timer_switch;

public:
	EnemyHornetThrowSwordState();
	~EnemyHornetThrowSwordState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [ÈÓ´ÌÇò]
class EnemyHornetThrowBarbsState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetThrowBarbsState();
	~EnemyHornetThrowBarbsState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [ÈÓË¿Ïß]
class EnemyHornetThrowSilkState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetThrowSilkState();
	~EnemyHornetThrowSilkState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [ËÀÍö]
class EnemyHornetDeadState : public StateNode
{
private:
	Timer timer_exit;
	Timer timer_dialogue;

public:
	EnemyHornetDeadState();
	~EnemyHornetDeadState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta);
};
