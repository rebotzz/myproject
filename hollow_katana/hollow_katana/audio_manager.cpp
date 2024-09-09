#include "audio_manager.h"
#include <condition_variable>
#include "util.h"


// ����ģʽ,������߳�ʱinstance()�̲߳���ȫ,��main֮ǰ��ʼ��,��û�ж��߳�
AudioManager* AudioManager::manager = new AudioManager;

AudioManager::AudioManager()
{
	player = new std::thread([&]()
		{
			while (true)
			{
				// Ϊ�˱�����������,����ͬ���������
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

// �����������������,���������������
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

// �������ʽ��������Ӱ����Ϸ, todo:��������
// ����player�̴߳����ʱ�������ȴ�,�����߳�Ƶ��������,�б�Ҫ��?
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

