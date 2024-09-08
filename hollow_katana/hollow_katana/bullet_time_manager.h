#pragma once
#include <easyx.h>

// 子弹时间类: 拦截器,拦截现实时间,返回游戏时间
class BulletTimeManager
{
public:
	enum class Status
	{
		Enter, Exit
	};

private:
	static BulletTimeManager* manager;

private:
	const float SPEED_PROGRESS = 2.0f;					// 进入子弹时间速度
	const float DST_DELTA_FACTOR = 0.35f;				// 最终子弹时间放慢系数
	const float DST_COLOR_FACTOR = 0.35f;				// 最终子弹时间全屏变暗系数
	float progress = 0.0f;								// 进度[0, 1]
	Status status = Status::Exit;

private:
	BulletTimeManager() = default;
	~BulletTimeManager() = default;

public:
	static BulletTimeManager* instance();
	void set_status(Status status);

	// 返回缩放后的时间
	float on_update(float delta);

	// 全屏变暗的后处理特效
	void post_progress();

private:
	// 线性插值
	float lerp(float start, float end, float progress);
};

