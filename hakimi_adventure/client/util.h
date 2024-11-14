#pragma once
#pragma comment(lib, "WINmm.lib")		// mciSendString() 
#pragma comment(lib, "MSIMG32.LIB")		// AlphaBlend()

#include <graphics.h>
#include "camera.h"

// 矩形区域
struct Rect
{
	int x, y;
	int w, h;
};

// 从原图片裁剪指定绘制,可以缩放图形
inline void putimage_alpha_ex(const Camera& camera, IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
	static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	const Vector2& pos_camera = camera.get_position();
	AlphaBlend(GetImageHDC(GetWorkingImage()), rect_dst->x - (int)pos_camera.x, rect_dst->y - (int)pos_camera.y,
		rect_dst->w, rect_dst->h, GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
		rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);
}

inline void load_audio(LPCTSTR path, LPCTSTR id)
{
	static TCHAR buff[256] = { 0 };
	_stprintf_s(buff, _T("open %s alias %s"), path, id);
	mciSendString(buff, nullptr, 0, nullptr);
}

inline void play_audio(LPCTSTR id, bool is_loop = false)
{
	// 为了避免延时,使用异步模式async
	static TCHAR buff[256] = { 0 };
	_stprintf_s(buff, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
	mciSendString(buff, nullptr, 0, nullptr);
}

inline void stop_audio(LPCTSTR id)
{
	static TCHAR buff[256] = { 0 };
	_stprintf_s(buff, _T("stop %s"), id);
	mciSendString(buff, nullptr, 0, nullptr);
}

