#pragma once
#include <vector>
#include <easyx.h>
#include "timer.h"
#include "animation.h"

#define REVERSE_TIME_VERSION_2

#ifdef REVERSE_TIME_VERSION_1
// 回溯时间管理器1.0 记录历史画面版本

class ReverseTimeManager
{
private:
	static ReverseTimeManager* manager;

private:
	// 缓慢进入效果控制
	const float FRAME_CD = 0.35f;							// 捕获屏幕画面cd,不能太大,才几分钟内存就用了1G
	const float SPEED_PROGRESS = 0.3f;						// 进入加速播放速度
	const float DST_DELTA_FACTOR = 5.0f;					// 最终播放速度倍数

private:
	Animation animation;									// 回放播放器
	std::vector<IMAGE> history_frame_list;					// 记录历史画面
	Timer timer_record_frame;								// 记录的时间间隔
	bool is_reverse = false;
	bool is_record = true;
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
	void set_record(bool flag)
	{
		is_record = flag;
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

	void update_play_speed(float delta);
};

#endif	// REVERSE_TIME_VERSION_1


#ifdef REVERSE_TIME_VERSION_2
// 回溯时间管理器2.0  记录历史信息,然后渲染

// 历史状态信息,用于时间回溯
// 记录动画帧,以及动画帧所在的位置
// 目前只记录玩家和boss, 未记录:粒子特效,角色召唤技能,角色武器动画等,需要单独提供接口

class ReverseTimeManager
{
private:
	static ReverseTimeManager* manager;

private:
	// 缓慢进入效果控制
	const float FRAME_CD = 0.2f;									// 捕获cd
	const float SPEED_PROGRESS = 0.3f;								// 进入加速播放速度
	const float DST_DELTA_FACTOR = 5.0f;							// 最终播放速度倍数

private:
	std::vector <std::vector<HistoryStatus>> history_status_list;	// 记录历史画面信息
	Timer timer_record_status;										// 记录的时间间隔
	Timer timer_play_history_record;								// 历史回放
	int idx_status = 0;												// 状态索引
	IMAGE* background = nullptr;									// 背景图
	bool is_reverse = false;
	bool is_record = true;
	bool is_enable = true;
	float progress = 0.0f;											// 进度[0, 1]

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
	void set_record(bool flag)
	{
		is_record = flag;
	}

	bool check_over() const
	{
		return !is_reverse;
	}

	void clear()
	{
		is_reverse = false;
		progress = 0.0f;
		history_status_list.clear();
	}

	void skip();

	void set_background(IMAGE* img)
	{
		background = img;
	}

private:
	void capture_current_status();

	void update_play_speed(float delta);

	void render_background();
};

#endif // REVERSE_TIME_VERSION_2