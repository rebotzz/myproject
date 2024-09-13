#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <string>
#include <Windows.h>

// 为了避免mciSendString()可能的阻塞,使用单独线程播放
// mciSendString()的所有操作需要在单个线程下完成
class AudioManager
{
private:
	struct AudioInfo
	{
		std::wstring path;
		std::wstring id;
	};

	struct AudioPlayOption
	{
		std::wstring id;
		bool is_loop;
	};

private:
	static AudioManager* manager;

private:
	// 任务队列
	std::queue<AudioInfo> load_queue;
	std::queue<AudioPlayOption> play_queue;
	std::queue<std::wstring> stop_queue;
	std::queue<std::wstring> resume_queue;
	std::queue<std::wstring> pause_queue;
	// 缓冲区
	std::queue<AudioInfo> load_queue_buffer;
	std::queue<AudioPlayOption> play_queue_buffer;
	std::queue<std::wstring> stop_queue_buffer;
	std::queue<std::wstring> resume_queue_buffer;
	std::queue<std::wstring> pause_queue_buffer;
	// 线程,锁,条件变量
	std::thread* audio_player = nullptr;
	std::mutex mtx;
	std::condition_variable cond;

private:
	AudioManager();
	~AudioManager();

public:
	static AudioManager* instance();
	void load_audio_ex(const std::wstring path, const std::wstring id);
	void play_audio_ex(const std::wstring id, bool is_loop = false);
	void stop_audio_ex(const std::wstring id);
	void pause_audio_ex(const std::wstring id);
	void resume_audio_ex(const std::wstring id);
};

