#include "bullet_time_manager.h"


BulletTimeManager* BulletTimeManager::manager = nullptr;

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

	return delta * lerp(1.0f, DST_DELTA_FACTOR, progress);
}

// 全屏变暗的后处理特效
void BulletTimeManager::post_progress()
{
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

void BulletTimeManager::set_status(Status status)
{
	this->status = status;
}

float BulletTimeManager::lerp(float start, float end, float progress)
{
	return start + progress * (end - start);
}