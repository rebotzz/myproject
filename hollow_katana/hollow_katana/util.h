#pragma once
#pragma comment(lib, "WINmm.lib")		// mciSendString() 
#pragma comment(lib, "MSIMG32.LIB")		// AlphaBlend()

#include <vector>
#include <easyx.h>
#include "camera.h"

#include <iostream>
using std::cout;
using std::endl;


// ���ⰴ����
constexpr int VK_A = 0x41;
constexpr int VK_W = 0x57;
constexpr int VK_S = 0x53;
constexpr int VK_D = 0x44;
constexpr int VK_J = 0x4A;
constexpr int VK_K = 0x4B;
constexpr int VK_L = 0x4C;
constexpr int VK_F = 0x46;
constexpr int VK_G = 0x47;
constexpr int VK_R = 0x52;
constexpr int VK_Z = 0x5A;
constexpr int VK_X = 0x58;
constexpr int VK_C = 0x43;
constexpr int VK_V = 0x56;


// ��ʱ���������
extern Camera* main_camera;

// ��������
struct Rect
{
	int x, y;
	int w, h;
};

// ��ʱ��¼��ʷ��Ϣ����
struct HistoryStatus
{
public:
	IMAGE* img;
	Rect rect_dst;
	Rect rect_src;
};

enum class Direction
{
	Up, Down, Left, Right
};


// ��ԭͼƬ�ü�ָ������,��������ͼ��
inline void putimage_alpha_ex(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
	// ��ʱ���������
	int x_dst = rect_dst->x - (main_camera ? (int)main_camera->get_position().x : 0);
	int y_dst = rect_dst->y - (main_camera ? (int)main_camera->get_position().y : 0);

	// ��Ⱦ�߼�
	static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(GetImageHDC(nullptr), x_dst, y_dst, rect_dst->w, rect_dst->h,
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

inline void load_audio(LPCTSTR path, LPCTSTR id)
{
	static TCHAR buff[256] = { 0 };
	_stprintf_s(buff, _T("open %s alias %s"), path, id);
	mciSendString(buff, nullptr, 0, nullptr);
}

inline void play_audio(LPCTSTR id, bool is_loop = false)
{
	// Ϊ�˱�����ʱ,ʹ���첽ģʽasync
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

inline void pause_audio(LPCTSTR id)
{
	static TCHAR buff[256] = { 0 };
	_stprintf_s(buff, _T("pause %s"), id);
	mciSendString(buff, nullptr, 0, nullptr);
}

inline void resume_audio(LPCTSTR id)
{
	static TCHAR buff[256] = { 0 };
	_stprintf_s(buff, _T("resume %s"), id);
	mciSendString(buff, nullptr, 0, nullptr);
}

inline int random_range(int min_num, int max_num)
{
	return min_num + rand() % (max_num - min_num + 1);
}


