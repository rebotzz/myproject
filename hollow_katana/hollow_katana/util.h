#pragma once
#include <easyx.h>

#pragma comment(lib, "WINmm.lib")		// mciSendString() 
#pragma comment(lib, "MSIMG32.LIB")		// AlphaBlend()

// ��������
struct Rect
{
	int x, y;
	int w, h;
};

// ��ԭͼƬ�ü�ָ������
inline void putimage_alpha_ex(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
	static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(GetImageHDC(nullptr), rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,
		GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
		rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);
}


// ʹ��ϵͳ��ʵ��͸��ͨ��ͼƬ����, easyX��putimage����ʵ��
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(nullptr), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
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