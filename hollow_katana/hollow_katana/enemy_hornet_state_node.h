#pragma once
#include "state_node.h"
#include "timer.h"

// ´ó»Æ·ä×´Ì¬½Úµã:
// 1.ÏĞÖÃ, 2.ÌøÔ¾, 3.±¼ÅÜ, 4.ÏÂÂä 5.¿ÕÖĞ³å´Ì 6.µØÃæ³å´Ì
// 7.Ãé×¼ 8.ÈÓ½£, 9.ÈÓ´ÌÇò 10.ÈÓË¿Ïß 11.ÏÂ¶× 12.ËÀÍö


// ´ó»Æ·äAIÂß¼­×´Ì¬½Úµã 1.0
#if 0

namespace hornet_state_version_1
{
	// [ÏĞÖÃ]¿ÉÌø×ª×´Ì¬: ËÀÍö ÌøÔ¾ ±¼ÅÜ Ãé×¼ ÈÓ½£ ÈÓ´ÌÇò ÈÓË¿Ïß
	class EnemyHornetIdleState : public StateNode
	{
	private:
		Timer timer;
		const float CD = 0.75f;
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

	// [±¼ÅÜ]¿ÉÌø×ª×´Ì¬: ËÀÍö ÏĞÖÃ ÌøÔ¾ Ãé×¼ ÈÓ½£ ÈÓË¿Ïß	->¿ÉÒÆ¶¯½Ó½üÍæ¼Ò
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
		const float CD = 0.55f;

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
}

#endif



// ´ó»Æ·äAIÂß¼­×´Ì¬½Úµã 2.0
// ²ßÂÔ: 1.°ëÑªÒÔÉÏ[²àÖØ½üÕ½¹¥»÷] 2.°ëÑªÒÔÏÂ[²àÖØÔ¶³Ì¹¥»÷] 

// [ÏĞÖÃ]
class EnemyHornetIdleState : public StateNode
{
private:
	Timer timer;
	const float MIN_DIS = 300.0f;

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
public:
	EnemyHornetDeadState() = default;
	~EnemyHornetDeadState() = default;
	virtual void on_enter()  override;
};
