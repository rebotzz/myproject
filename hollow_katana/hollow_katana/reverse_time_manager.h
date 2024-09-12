#pragma once
#include <vector>
#include <easyx.h>
#include "timer.h"
#include "animation.h"

// ����ʱ�������

class ReverseTimeManager
{
private:
	static ReverseTimeManager* manager;

private:
	// ��������Ч������
	const float FRAME_CD = 0.1f;							// ������Ļ����cd
	const float SPEED_PROGRESS = 0.5f;						// ������ٲ����ٶ�,2s
	const float DST_DELTA_FACTOR = 3.0f;					// ���ղ���ϵ��,3����

private:
	Animation animation;									// �طŲ�����
	std::vector<IMAGE> history_frame_list;					// ��¼��ʷ����
	Timer timer_record_frame;								// ��¼��ʱ����
	bool is_reverse = false;
	bool is_enable = true;
	float progress = 0.0f;									// ����[0, 1]

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