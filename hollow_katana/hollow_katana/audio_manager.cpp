#include "audio_manager.h"
#include <condition_variable>
#include "util.h"


// ����ģʽ,������߳�ʱinstance()�̲߳���ȫ,��main֮ǰ��ʼ��,��û�ж��߳�
AudioManager* AudioManager::manager = new AudioManager;

AudioManager::AudioManager()
{
	audio_player = new std::thread([&]()
		{
			while (true)
			{
				{
					// Ϊ�˱�����������,����ͬ���������
					// ֻ���ڽ�����������������
					std::unique_lock<std::mutex> lock(mtx);
					cond.wait(lock);

					if (load_queue.empty() && !load_queue_buffer.empty())
						load_queue.swap(load_queue_buffer);

					if (play_queue.empty() && !play_queue_buffer.empty())
						play_queue.swap(play_queue_buffer);

					if (stop_queue.empty() && !stop_queue_buffer.empty())
						stop_queue.swap(stop_queue_buffer);

					if (resume_queue.empty() && !resume_queue_buffer.empty())
						resume_queue.swap(resume_queue_buffer);

					if (pause_queue.empty() && !pause_queue_buffer.empty())
						pause_queue.swap(pause_queue_buffer);
				}


				while (!load_queue.empty())
				{
					auto front = load_queue.front();
					load_queue.pop();
					load_audio(front.path.c_str(), front.id.c_str());
				}

				while (!play_queue.empty())
				{
					auto front = play_queue.front();
					play_queue.pop();
					play_audio(front.id.c_str(), front.is_loop);
				}

				while (!stop_queue.empty())
				{
					auto front = stop_queue.front();
					stop_queue.pop();
					stop_audio(front.c_str());
				}

				while (!resume_queue.empty())
				{
					auto front = resume_queue.front();
					resume_queue.pop();
					resume_audio(front.c_str());
				}

				while (!pause_queue.empty())
				{
					auto front = pause_queue.front();
					pause_queue.pop();
					pause_audio(front.c_str());
				}
			}
		});
}

// �����������������,�����������
AudioManager::~AudioManager()
{
	if (audio_player)
	{
		audio_player->join();
		delete audio_player;
	}
}

AudioManager* AudioManager::instance()
{
	return manager;
}

// �������ʽ��������Ӱ����Ϸ, todo:��������
// ����player�̴߳����ʱ�������ȴ�,�����߳�Ƶ��������,�б�Ҫ��?
// ����,����ʱ������Ҳ��,��ʧ��Ƶ�ܱ���Ϸ���ٺ�
void AudioManager::load_audio_ex(const std::wstring path, const std::wstring id)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		load_queue_buffer.push({ path, id });
	}

	cond.notify_one();
}

void AudioManager::play_audio_ex(const std::wstring id, bool is_loop)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		play_queue_buffer.push({ id, is_loop });
	}

	cond.notify_one();
}

void AudioManager::stop_audio_ex(const std::wstring id)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		stop_queue_buffer.push(id);
	}

	cond.notify_one();
}

void AudioManager::pause_audio_ex(const std::wstring id)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		pause_queue_buffer.push(id);
	}

	cond.notify_one();
}

void AudioManager::resume_audio_ex(const std::wstring id)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		resume_queue_buffer.push(id);
	}

	cond.notify_one();
}
