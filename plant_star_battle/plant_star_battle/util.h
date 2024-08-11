#pragma once
#include <easyx.h>
#include <cassert>
#include "camera.h"

#pragma comment(lib, "WINmm.lib")	// mciSendString() ý����ƽӿ�
#pragma comment(lib, "MSIMG32.LIB")
// ʹ��ϵͳ��ʵ��͸��ͨ��ͼƬ����, easyX��putimage����ʵ��
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

// ����ü�����
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

//���ҷ�תͼƬ, ����زĸ���
inline void flip_image(IMAGE* src, IMAGE* dst)
{
	int width = src->getwidth(), height = src->getheight();
	dst->Resize(width, height);
	DWORD* img_src_buffer = GetImageBuffer(src);
	DWORD* img_dst_buffer = GetImageBuffer(dst);
	// image���߼����������ص�(R,G,B)���ɵĶ�ά����,�ڴ�����һά����
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

// ���ɽ�ɫ��ɫ��ӰͼƬ
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
			// ����Դ͸��ͨ������,��ɫ��Ϊ��ɫ
			int idx = y * width + x;
			img_dst_buffer[idx] = BGR(RGB(255, 255, 255)) | (img_src_buffer[idx] & 0xff000000);	
		}
	}
}