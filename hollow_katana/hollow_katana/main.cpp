#include <iostream>
#include <chrono>
#include <thread>
#include <easyx.h>
#include "util.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "bullet_time_manager.h"
#include "audio_manager.h"

#include "scene_manager.h"

using std::cout;
using std::endl;

const int WINDOW_WIDTH = 1270;
const int WINDOW_HEIGHT = 720;
const int FPS = 144;
const Camera* main_camera = nullptr;

int main()
{
	// ��ʼ��
	HWND hwnd = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EX_SHOWCONSOLE);
	SetWindowText(hwnd, _T("Hollow Katana"));
	BeginBatchDraw();
	ExMessage msg;
	bool is_quit = false;

	try
	{
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR& id)
	{
		TCHAR error_msg[256] = { 0 };
		_stprintf_s(error_msg, _T("�޷�����: %s"), id);
		MessageBox(hwnd, error_msg, _T("��Դ����ʧ��"), MB_OK | MB_ICONERROR);
		return -1;
	}

	using namespace std::chrono;
	const nanoseconds frame_duration((int)1e9 / FPS);
	steady_clock::time_point last_tick = steady_clock::now();

	AudioManager::instance()->play_audio_ex(_T("bgm"), true);
	main_camera = &(SceneManager::instance()->get_camera());
	SceneManager::instance()->set_entry_scene("game_scene_boss_hornet");

	try
	{
		// ��ѭ��
		while (!is_quit)
		{
			// ������Ϣ
			if (peekmessage(&msg, EX_MOUSE | EX_KEY))
			{
				SceneManager::instance()->on_input(msg);
			}

			steady_clock::time_point frame_start = steady_clock::now();
			duration<float> delta = duration<float>(frame_start - last_tick);

			// �������
			float scaled_delta = BulletTimeManager::instance()->on_update(delta.count());

			SceneManager::instance()->on_update(scaled_delta);
			CollisionManager::instance()->process_collide();

			// �����ͼ
			cleardevice();

			SceneManager::instance()->on_render();
			CollisionManager::instance()->on_debug_render();

			FlushBatchDraw();


			// ��̬��ʱ
			last_tick = frame_start;
			nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
			if (sleep_duration > nanoseconds(0))
				std::this_thread::sleep_for(sleep_duration);
		}
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}

	closegraph();
	EndBatchDraw();

	return 0;
}
