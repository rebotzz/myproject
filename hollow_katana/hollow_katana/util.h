#pragma once
#include <easyx.h>

#pragma comment(lib, "WINmm.lib")		// mciSendString() 
#pragma comment(lib, "MSIMG32.LIB")		// AlphaBlend()

// 矩形区域
struct Rect
{
	int x, y;
	int w, h;
};

// 从原图片裁剪指定绘制
inline void putimage_alpha_ex(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
	static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(GetImageHDC(nullptr), rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,
		GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
		rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);
}


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