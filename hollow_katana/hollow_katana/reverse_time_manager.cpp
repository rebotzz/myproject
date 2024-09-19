#include "reverse_time_manager.h"
#include "scene_manager.h"

#ifdef REVERSE_TIME_VERSION_2
#include "character_manager.h"
#endif

#ifdef REVERSE_TIME_VERSION_1

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
			// 生成黑色斑驳粒子特效, 粒子发生器
		}

		if (!is_reverse && is_record)
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

void ReverseTimeManager::update_play_speed(float delta)
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
	float current_frame_cd = FRAME_CD / speed_ratio;
	animation.set_interval(current_frame_cd);
}

#endif // REVERSE_TIME_VERSION_1


#ifdef REVERSE_TIME_VERSION_2

ReverseTimeManager* ReverseTimeManager::manager = nullptr;
ReverseTimeManager::ReverseTimeManager()
{
	timer_record_status.set_one_shot(false);
	timer_record_status.set_wait_time(FRAME_CD);
	timer_record_status.set_on_timeout([&]
		{
			// 获取当前时间点信息
			capture_current_status();
		});

	timer_play_history_record.set_one_shot(false);
	timer_play_history_record.set_wait_time(FRAME_CD);
	timer_play_history_record.set_on_timeout([&]
		{
			if (idx_status >= 0)
			{
				idx_status--;
			}
			else
			{
				clear();
				is_enable = false;
				SceneManager::instance()->switch_to_pre_scene();
			}

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
		update_play_speed(delta);

		if (is_reverse)
		{
			timer_play_history_record.on_update(delta);
		}

		if (!is_reverse && is_record)
			timer_record_status.on_update(delta);
	}
}

void ReverseTimeManager::on_render()
{
	if (is_enable && is_reverse && idx_status >= 0)
	{
		// 背景
		render_background();

		// 玩家, 敌人
		auto& player = history_status_list[idx_status][0];
		auto& enemy = history_status_list[idx_status][1];
		putimage_alpha_ex(player.img, &player.rect_dst, &player.rect_src);
		putimage_alpha_ex(enemy.img, &enemy.rect_dst, &enemy.rect_src);
	}
}


void ReverseTimeManager::reverse_time()
{
	if (history_status_list.empty() || is_reverse)
		return;

	is_enable = true;
	is_reverse = true;
	idx_status = (int)history_status_list.size() - 1;
}

void ReverseTimeManager::capture_current_status()
{
	history_status_list.push_back(
		{
			CharacterManager::instance()->get_player()->get_current_status(),
			CharacterManager::instance()->get_enemy()->get_current_status(),
		}
		);
}


void ReverseTimeManager::skip()
{
	clear();
	is_enable = false;
	SceneManager::instance()->switch_to_pre_scene();
}

void ReverseTimeManager::update_play_speed(float delta)
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
	float current_frame_cd = FRAME_CD / speed_ratio;
	timer_play_history_record.set_wait_time(current_frame_cd);
}

void ReverseTimeManager::render_background()
{
	if (nullptr == background)
		return;

	static Rect rect_dst;
	rect_dst.x = (getwidth() - rect_dst.w) / 2;
	rect_dst.y = (getheight() - rect_dst.h) / 2;
	rect_dst.w = background->getwidth(), rect_dst.h = background->getheight();

	putimage_alpha_ex(background, &rect_dst);
}

#endif  // REVERSE_TIME_VERSION_2