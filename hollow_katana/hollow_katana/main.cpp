#pragma comment (linker,"/subsystem:windows /entry:mainCRTStartup")		// 关闭控制台窗口

#include <chrono>
#include <thread>
#include "util.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "bullet_time_manager.h"
#include "audio_manager.h"
#include "scene_manager.h"


const int WINDOW_WIDTH = 1270;
const int WINDOW_HEIGHT = 720;
const int FPS = 144;
Camera* main_camera = nullptr;

int main()
{
	// 初始化
	HWND hwnd = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
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
		_stprintf_s(error_msg, _T("无法加载: %s"), id);
		MessageBox(hwnd, error_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
		return -1;
	}

	using namespace std::chrono;
	const nanoseconds frame_duration((int)1e9 / FPS);
	steady_clock::time_point last_tick = steady_clock::now();
	main_camera = SceneManager::instance()->get_camera();
	SceneManager::instance()->set_entry_scene("menu_scene");	//menu_scene game_scene_choice 

	// 主循环
	while (!is_quit)
	{
		// 处理消息		debug:改为while,逐个拉取事件队列
		while (peekmessage(&msg, EX_MOUSE | EX_KEY))
		{
			SceneManager::instance()->on_input(msg);
		}

		steady_clock::time_point frame_start = steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_tick);

		// 处理更新
		float scaled_delta = BulletTimeManager::instance()->on_update(delta.count());
		SceneManager::instance()->on_update(scaled_delta);
		CollisionManager::instance()->process_collide();

		// 处理绘图
		cleardevice();
		SceneManager::instance()->on_render();
		//CollisionManager::instance()->on_debug_render();
		FlushBatchDraw();

		// 动态延时
		last_tick = frame_start;
		nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
		if (sleep_duration > nanoseconds(0))
			std::this_thread::sleep_for(sleep_duration);
	}


	closegraph();
	EndBatchDraw();

	return 0;
}
