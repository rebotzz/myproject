#include "reverse_time_manager.h"
#include "scene_manager.h"

ReverseTimeManager* ReverseTimeManager::manager = nullptr;
ReverseTimeManager::ReverseTimeManager()
{
	timer_record_frame.set_one_shot(false);
	timer_record_frame.set_wait_time(FRAME_CD);
	timer_record_frame.set_on_timeout([&]
		{
			capture_current_frame();
		});

	animation.set_loop(false);
	animation.set_interval(FRAME_CD);
	animation.set_achor_mode(Animation::AchorMode::Centered);
	animation.set_position({ (float)getwidth() / 2, (float)getheight() / 2 });
	animation.set_on_finished([&]
		{
			clear();
			is_enable = false;
			SceneManager::instance()->switch_to_pre_scene();
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
	if (is_enable)
	{
		update_player_speed(delta);

		if (is_reverse && !animation.empty())
		{
			animation.on_update(delta);

			// todo:
			// 播放时间逆流音效
			// 生成黑色斑驳粒子特效, 粒子发生器
		}

		if (!is_reverse)
			timer_record_frame.on_update(delta);
	}
}

void ReverseTimeManager::on_render()
{
	if (is_enable && is_reverse)
		animation.on_render();
}


void ReverseTimeManager::reverse_time()
{
	if (history_frame_list.empty() || is_reverse)
		return;

	is_enable = true;
	is_reverse = true;

	for (int i = (int)history_frame_list.size() - 1; i >= 0; --i)
	{
		animation.add_frame(&history_frame_list[i], 1);
	}
}

void ReverseTimeManager::capture_current_frame()
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

	history_frame_list.push_back(record_frame);
}


void ReverseTimeManager::skip()
{
	// todo: 加快 播放速度
	clear();
	is_enable = false;
	SceneManager::instance()->switch_to_pre_scene();
}

void ReverseTimeManager::update_player_speed(float delta)
{
	if (progress == 1.0)
		return;

	float progress_delta = SPEED_PROGRESS * delta;
	progress += progress_delta;
	if (progress < 0.0) progress = 0.0;
	else if (progress > 1.0) progress = 1.0;

	auto lerp = [](float start, float end, float progress)
		{
			return start + progress * (end - start);
		};

	float speed_ratio = lerp(1.0, DST_DELTA_FACTOR, progress);
	float current_frame_cd = delta * FRAME_CD / speed_ratio;
	animation.set_interval(current_frame_cd);
}