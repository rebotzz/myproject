#include "audio_manager.h"
#include <condition_variable>
#include "util.h"


// 饿汉模式,避免多线程时instance()线程不安全,在main之前初始化,还没有多线程
AudioManager* AudioManager::manager = new AudioManager;

AudioManager::AudioManager()
{
	player = new std::thread([&]()
		{
			while (true)
			{
				// 为了避免性能消耗,加入同步互斥机制
				std::unique_lock<std::mutex> lock(mtx);
				cond.wait(lock);

				while (!load_queue.empty())
				{
					auto front = load_queue.front();
					load_queue.pop();
					load_audio(front.path, front.id);
				}

				while (!play_queue.empty())
				{
					auto front = play_queue.front();
					play_queue.pop();
					play_audio(front.id, front.is_loop);
				}

				while (!stop_queue.empty())
				{
					auto front = stop_queue.front();
					stop_queue.pop();
					stop_audio(front);
				}
			}
		});
}

// 单例生命周期随进程,所有析构不会调用
AudioManager::~AudioManager()
{
	if (player)
	{
		player->join();
		delete player;
	}
}

AudioManager* AudioManager::instance()
{
	return manager;
}

// 这里堵塞式申请锁会影响游戏, todo:无锁队列
// 但是player线程大多数时候阻塞等待,就主线程频繁申请锁,有必要吗?
void AudioManager::load_audio_ex(LPCTSTR path, LPCTSTR id)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		load_queue.push({ path, id });
	}

	cond.notify_one();
}

void AudioManager::play_audio_ex(LPCTSTR id, bool is_loop)
{
	{
		//std::unique_lock<std::mutex> lock(mtx);
		play_queue.push({ id, is_loop });
	}

	cond.notify_one();
}

void AudioManager::stop_audio_ex(LPCTSTR id)
{
	{
		//std::unique_lock<std::mutex> lock(mtx);
		stop_queue.push(id);
	}

	cond.notify_one();
}

