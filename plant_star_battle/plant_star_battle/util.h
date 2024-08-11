#pragma once
#include <easyx.h>
#include <cassert>
#include "camera.h"

#pragma comment(lib, "WINmm.lib")	// mciSendString() 媒体控制接口
#pragma comment(lib, "MSIMG32.LIB")
// 使用系统库实现透明通道图片加载, easyX的putimage不能实现
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(nullptr), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void putimage_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	Vector2 pos_camera = camera.get_position();
	AlphaBlend(GetImageHDC(nullptr), (int)(dst_x - pos_camera.x), (int)(dst_y - pos_camera.y), w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

// 区域裁剪绘制
inline void putimage_alpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y)
{
	int w = width > 0 ? width : img->getwidth();
	int h = height > 0 ? height : img->getheight();
	AlphaBlend(GetImageHDC(nullptr), dst_x, dst_y, w, h,
		GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2)
{
	const Vector2& pos_camera = camera.get_position();
	line(x1 - (int)pos_camera.x, y1 - (int)pos_camera.y, x2 - (int)pos_camera.x, y2 - (int)pos_camera.y);
}

inline void rectangle(const Camera& camera, int left, int  top, int  right, int  bottom)
{
	const Vector2& pos_camera = camera.get_position();
	rectangle(left - (int)pos_camera.x, top - (int)pos_camera.y, right - (int)pos_camera.x, bottom - (int)pos_camera.y);
}

inline void solidroundrect(const Camera& camera, int left, int  top, int  right, int  bottom, int ellipsewidth, int ellipseheight)
{
	const Vector2& pos_camera = camera.get_position();
	solidroundrect(left - (int)pos_camera.x, top - (int)pos_camera.y, right - (int)pos_camera.x, bottom - (int)pos_camera.y, ellipsewidth, ellipseheight);
}

inline void	solidcircle(const Camera& camera, int x, int y, int radius)
{
	const Vector2& pos_camera = camera.get_position();
	solidcircle(x - (int)pos_camera.x, y - (int)pos_camera.y, radius);
}

//左右翻转图片, 提高素材复用
inline void flip_image(IMAGE* src, IMAGE* dst)
{
	int width = src->getwidth(), height = src->getheight();
	dst->Resize(width, height);
	DWORD* img_src_buffer = GetImageBuffer(src);
	DWORD* img_dst_buffer = GetImageBuffer(dst);
	// image在逻辑上是由像素点(R,G,B)构成的二维数组,内存上是一维数组
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int src_idx = y * width + width - 1 - x;
			int dst_idx = y * width + x;
			img_dst_buffer[dst_idx] = img_src_buffer[src_idx];
		}
	}
}

// 生成角色白色剪影图片
inline void sketch_image(IMAGE* src, IMAGE* dst)
{
	int width = src->getwidth(), height = src->getheight();
	dst->Resize(width, height);
	DWORD* img_src_buffer = GetImageBuffer(src);
	DWORD* img_dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// 保持源透明通道不变,颜色改为白色
			int idx = y * width + x;
			img_dst_buffer[idx] = BGR(RGB(255, 255, 255)) | (img_src_buffer[idx] & 0xff000000);	
		}
	}
}