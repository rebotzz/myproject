#include "bullet_time_manager.h"
#include "audio_manager.h"


BulletTimeManager* BulletTimeManager::manager = nullptr;

BulletTimeManager::BulletTimeManager()
{
	timer_play_audio.set_one_shot(true);
	timer_play_audio.set_wait_time(3.0f);
	timer_play_audio.set_on_timeout([&]
		{
			is_play_cd_comp = true;
		});
}


BulletTimeManager* BulletTimeManager::instance()
{
	if (nullptr == manager)
		manager = new BulletTimeManager;

	return manager;
}

// 返回缩放后的时间
float BulletTimeManager::on_update(float delta)
{
	float progress_delta = SPEED_PROGRESS * delta;
	progress += (status == Status::Enter ? progress_delta : -progress_delta);

	if (progress < 0.0)
		progress = 0.0;
	else if (progress > 1.0)
		progress = 1.0;

	timer_play_audio.on_update(delta);

	return delta * lerp(1.0f, DST_DELTA_FACTOR, progress);
}

// 全屏变暗的后处理特效
void BulletTimeManager::post_progress()
{
	if (progress < 0.001f)
		return;

	DWORD* img_buffer = GetImageBuffer();
	int width = getwidth();
	int height = getheight();

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int idx = y * width + x;
			COLORREF color = img_buffer[idx];
			BYTE r = (BYTE)(GetBValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
			BYTE g = (BYTE)(GetGValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
			BYTE b = (BYTE)(GetRValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
			img_buffer[idx] = BGR(RGB(r, g, b)) | ((DWORD)((BYTE)255) << 24);
		}
	}
}



#include <iostream>
void BulletTimeManager::set_status(Status status)
{
	this->status = status;

	if (status == Status::Enter && !is_play_audio && is_play_cd_comp)
	{
		std::cout << "播放子弹时间音效" << std::endl;
		is_play_audio = true;
		is_play_cd_comp = false;
		timer_play_audio.restart();
		AudioManager::instance()->play_audio_ex(_T("bullet_time"));
	}
	else
		is_play_audio = false;
}

float BulletTimeManager::lerp(float start, float end, float progress)
{
	return start + progress * (end - start);
}

