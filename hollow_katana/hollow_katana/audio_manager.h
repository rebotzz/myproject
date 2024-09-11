#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <Windows.h>

// 为了避免mciSendString()可能的阻塞,使用单独线程播放
// mciSendString()的所有操作需要在单个线程下完成
class AudioManager
{
private:
	struct AudioInfo
	{
		LPCTSTR path;
		LPCTSTR id;
	};

	struct AudioPlayOption
	{
		LPCTSTR id;
		bool is_loop;
	};

private:
	static AudioManager* manager;

private:
	// 任务队列
	std::queue<AudioInfo> load_queue;
	std::queue<AudioPlayOption> play_queue;
	std::queue<LPCTSTR> stop_queue;
	// 缓冲区
	std::queue<AudioInfo> load_queue_buffer;
	std::queue<AudioPlayOption> play_queue_buffer;
	std::queue<LPCTSTR> stop_queue_buffer;
	// 线程,锁,条件变量
	std::thread* player = nullptr;
	std::mutex mtx;
	std::condition_variable cond;

private:
	AudioManager();
	~AudioManager();

public:
	static AudioManager* instance();
	void load_audio_ex(LPCTSTR path, LPCTSTR id);
	void play_audio_ex(LPCTSTR id, bool is_loop = false);
	void stop_audio_ex(LPCTSTR id);
};

