#pragma once
#include <vector>
#include <easyx.h>
#include "timer.h"
#include "animation.h"

// 回溯时间管理器

class ReverseTimeManager
{
private:
	static ReverseTimeManager* manager;

private:
	// 缓慢进入效果控制
	const float FRAME_CD = 0.1f;							// 捕获屏幕画面cd
	const float SPEED_PROGRESS = 0.5f;						// 进入加速播放速度,2s
	const float DST_DELTA_FACTOR = 3.0f;					// 最终播放系数,3倍数

private:
	Animation animation;									// 回放播放器
	std::vector<IMAGE> history_frame_list;					// 记录历史画面
	Timer timer_record_frame;								// 记录的时间间隔
	bool is_reverse = false;
	bool is_enable = true;
	float progress = 0.0f;									// 进度[0, 1]

private:
	ReverseTimeManager();
	~ReverseTimeManager();

public:
	static ReverseTimeManager* instance();

	void on_update(float delta);
	void on_render();
	void reverse_time();

	void set_enable(bool flag)
	{
		is_enable = true;
	}

	bool check_over() const
	{
		return !is_reverse;
	}

	void clear()
	{
		is_reverse = false;
		progress = 0.0f;
		animation.clear();
		history_frame_list.clear();
	}

	void skip();

private:
	void capture_current_frame();

	void update_player_speed(float delta);
};