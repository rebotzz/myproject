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
	const float FRAME_CD = 0.35f;							// ������Ļ����cd,����̫��,�ż������ڴ������1G
	const float SPEED_PROGRESS = 0.3f;						// ������ٲ����ٶ�
	const float DST_DELTA_FACTOR = 5.0f;					// ���ղ����ٶȱ���

private:
	Animation animation;									// �طŲ�����
	std::vector<IMAGE> history_frame_list;					// ��¼��ʷ����
	Timer timer_record_frame;								// ��¼��ʱ����
	bool is_reverse = false;
	bool is_record = true;
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

	void update_player_speed(float delta);
};