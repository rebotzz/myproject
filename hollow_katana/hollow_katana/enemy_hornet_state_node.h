#pragma once
#include "state_node.h"
#include "timer.h"

// ´ó»Æ·ä×´Ì¬½Úµã:
// 1.ÏĞÖÃ, 2.ÌøÔ¾, 3.±¼ÅÜ, 4.ÏÂÂä 5.¿ÕÖĞ³å´Ì 6.µØÃæ³å´Ì
// 7.Ãé×¼ 8.ÈÓ½£, 9.ÈÓ´ÌÇò 10.ÈÓË¿Ïß 11.ËÀÍö

// [ÏĞÖÃ]¿ÉÌø×ª×´Ì¬: ËÀÍö ÌøÔ¾ ±¼ÅÜ Ãé×¼ ÈÓ½£ ÈÓ´ÌÇò ÈÓË¿Ïß
class EnemyHornetIdleState : public StateNode
{
private:
	Timer timer;
	const float CD = 1.0f;
	bool can_switch_state = false;

public:
	EnemyHornetIdleState();
	~EnemyHornetIdleState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};


// [ÌøÔ¾]¿ÉÌø×ª×´Ì¬: ËÀÍö ÏÂÂä ¿ÕÖĞ³å´Ì ÈÓË¿Ïß  ->¿ÉÒÆ¶¯½Ó½üÍæ¼Ò
class EnemyHornetJumpState : public StateNode
{
private:
	bool is_jump_left = false;

public:
	EnemyHornetJumpState() = default;
	~EnemyHornetJumpState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [±¼ÅÜ]¿ÉÌø×ª×´Ì¬: ËÀÍö ÏĞÖÃ ÌøÔ¾ Ãé×¼ ÈÓË¿Ïß	->¿ÉÒÆ¶¯½Ó½üÍæ¼Ò
class EnemyHornetRunState : public StateNode
{
public:
	EnemyHornetRunState() = default;
	~EnemyHornetRunState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [ÏÂÂä]¿ÉÌø×ª×´Ì¬: ËÀÍö ÏĞÖÃ 	->¿ÉÒÆ¶¯½Ó½üÍæ¼Ò
class EnemyHornetFallState : public StateNode
{
public:
	EnemyHornetFallState() = default;
	~EnemyHornetFallState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};

// [¿ÕÖĞ³å´Ì]¿ÉÌø×ª×´Ì¬: ËÀÍö ÏĞÖÃ
class EnemyHornetDashAirState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetDashAirState();
	~EnemyHornetDashAirState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [µØÃæ³å´Ì]¿ÉÌø×ª×´Ì¬: ËÀÍö ÏĞÖÃ
class EnemyHornetDashFloorState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetDashFloorState();
	~EnemyHornetDashFloorState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [Ãé×¼]¿ÉÌø×ª×´Ì¬: ËÀÍö µØÃæ³å´Ì
class EnemyHornetAimState : public StateNode
{
private:
	Timer timer;
	const float CD = 0.6f;

public:
	EnemyHornetAimState();
	~EnemyHornetAimState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
};


// [ÈÓ½£]¿ÉÌø×ª×´Ì¬: ËÀÍö ÏĞÖÃ
class EnemyHornetThrowSwordState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetThrowSwordState();
	~EnemyHornetThrowSwordState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [ÈÓ´ÌÇò]¿ÉÌø×ª×´Ì¬: ËÀÍö ÏĞÖÃ
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

// [ÈÓË¿Ïß]¿ÉÌø×ª×´Ì¬: ËÀÍö ÏĞÖÃ
class EnemyHornetThrowSilkState : public StateNode
{
private:
	Timer timer;

public:
	EnemyHornetThrowSilkState();
	~EnemyHornetThrowSilkState() = default;
	virtual void on_enter()  override;
	virtual void on_update(float delta) override;
	virtual void on_exit()  override;
};

// [ËÀÍö]: ½áÊøÕ½¶·
class EnemyHornetDeadState : public StateNode
{
public:
	EnemyHornetDeadState() = default;
	~EnemyHornetDeadState() = default;
	virtual void on_enter()  override;
};
