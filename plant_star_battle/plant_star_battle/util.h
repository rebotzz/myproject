#pragma once
#include <easyx.h>
#include <cassert>
#include <iostream>

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

