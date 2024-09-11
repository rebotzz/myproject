#include "reverse_time_manager.h"


ReverseTimeManager* ReverseTimeManager::manager = nullptr;
ReverseTimeManager::ReverseTimeManager()
{
	timer_record_frame.set_one_shot(false);
	timer_record_frame.set_wait_time(0.2f);
	timer_record_frame.set_on_timeout([&]
		{
			IMAGE record_frame;
			int width = getwidth();
			int height = getheight();
			record_frame.Resize(width, height);

			DWORD* screen_buffer = GetImageBuffer();
			DWORD* record_buffer = GetImageBuffer(&record_frame);

			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					int idx = y * width + x;
					record_buffer[idx] = screen_buffer[idx];
				}
			}

			frame_list.push_back(record_frame);
		});

	animation.set_loop(false);
	animation.set_interval(0.02f);
	animation.set_achor_mode(Animation::AchorMode::Centered);
	animation.set_position({ (float)getwidth() / 2, (float)getheight() / 2 });
	animation.set_on_finished([&]
		{
			is_reverse = false;
			animation.clear();
			frame_list.clear();
		});
}

ReverseTimeManager::~ReverseTimeManager() = default;

ReverseTimeManager* ReverseTimeManager::instance()
{
	if (nullptr == manager)
		manager = new ReverseTimeManager;

	return manager;
}

void ReverseTimeManager::on_update(float delta)
{
	if (is_reverse)
	{
		animation.on_update(delta);

		// todo:
		// 播放时间逆流音效
		// 生成黑色斑驳粒子特效, 粒子发生器
	}
	else
		timer_record_frame.on_update(delta);
}

void ReverseTimeManager::on_render()
{
	if (is_reverse)
		animation.on_render();
}


void ReverseTimeManager::reverse_time()
{
	if (is_reverse)
		return;

	is_reverse = true;

	for (int i = (int)frame_list.size(); i >= 0; --i)
	{
		animation.add_frame(&frame_list[i], 1);
	}
}

void ReverseTimeManager::skip()
{
	// todo

}