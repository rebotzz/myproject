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
	Animation animation;
	std::vector<IMAGE> frame_list;				// ��¼��ʷ����
	Timer timer_record_frame;								// ��¼���
	bool is_reverse = false;

private:
	ReverseTimeManager();
	~ReverseTimeManager();

public:
	static ReverseTimeManager* instance();

	void on_update(float delta);
	void on_render();
	void reverse_time();

	bool check_over() const
	{
		return !is_reverse && frame_list.empty();
	}

	bool clear()
	{
		is_reverse = false;
		animation.clear();
		frame_list.clear();
	}

	void skip();
};