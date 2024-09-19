#pragma once
#include <vector>
#include <easyx.h>
#include "timer.h"
#include "animation.h"

#define REVERSE_TIME_VERSION_2

#ifdef REVERSE_TIME_VERSION_1
// ����ʱ�������1.0 ��¼��ʷ����汾

class ReverseTimeManager
{
private:
	static ReverseTimeManager* manager;

private:
	// ��������Ч������
	const float FRAME_CD = 0.35f;							// ������Ļ����cd,����̫��,�ż������ڴ������1G
	const float SPEED_PROGRESS = 0.3f;						// ������ٲ����ٶ�
	const float DST_DELTA_FACTOR = 5.0f;					// ���ղ����ٶȱ���

private:
	Animation animation;									// �طŲ�����
	std::vector<IMAGE> history_frame_list;					// ��¼��ʷ����
	Timer timer_record_frame;								// ��¼��ʱ����
	bool is_reverse = false;
	bool is_record = true;
	bool is_enable = true;
	float progress = 0.0f;									// ����[0, 1]

private:
	ReverseTimeManager();
	~ReverseTimeManager();

public:
	static ReverseTimeManager* instance();

	void on_update(float delta);
	void on_render();
	void reverse_time();

	void set_enable(bool flag)
	{
		is_enable = true;
	}
	void set_record(bool flag)
	{
		is_record = flag;
	}

	bool check_over() const
	{
		return !is_reverse;
	}

	void clear()
	{
		is_reverse = false;
		progress = 0.0f;
		animation.clear();
		history_frame_list.clear();
	}

	void skip();

private:
	void capture_current_frame();

	void update_play_speed(float delta);
};

#endif	// REVERSE_TIME_VERSION_1


#ifdef REVERSE_TIME_VERSION_2
// ����ʱ�������2.0  ��¼��ʷ��Ϣ,Ȼ����Ⱦ

// ��ʷ״̬��Ϣ,����ʱ�����
// 
// ����1:
// ��¼����֡,�Լ�����֡���ڵ�λ��
// �ڳ���update����¼��Ϣ,��Ҫcharacter,particle�ṩ��¼�ӿ�
// ȱ��: 
// 1.�ƻ��˶���֡�ķ�װ���� 
// 2.����ͨ��,��ͬ��ɫ���ٻ�����Ҳ��Ҫ��¼,��Ҫ�ػ�����
// 3.�ӵ�ʱ�������Ч���ܼ�¼
// 
// ����2:
// ��¼����֡,�Լ�����֡���ڵ�λ��
// �ڸ�Դ��putimage_alpha_ex()��¼��Ϣ�ƺ��Ϳ�����
// �������ʵʱ����(��������ģ��),ֻ��Ҫ����Ļ�
// ���Ҫ�����߼����,��ô��������Ⱦ����¼,����renderһ��Ƕ��һ��,�����û����Ϣ
// ȱ��: 
// 1.���ÿ��Ʋ���֡���, 
// 2.���ݴ������Ⱦ�߼����
// 3.��֪��һ��������Ⱦ�ж��ٸ�����,���׼�¼����,����,��Ҫ���λΪ�ָ�����֡��Ԫ��
// 4.�ӵ�ʱ�������Ч���ܼ�¼
//
// �ۺ�: ����һ��,ʹ�÷���1,���ǲ���¼������Ч,��ɫ�ٻ�����,��ɫ����������,ֻ��¼��Һ�boss

class ReverseTimeManager
{
private:
	static ReverseTimeManager* manager;

private:
	// ��������Ч������
	const float FRAME_CD = 0.2f;									// ����cd
	const float SPEED_PROGRESS = 0.3f;								// ������ٲ����ٶ�
	const float DST_DELTA_FACTOR = 5.0f;							// ���ղ����ٶȱ���

private:
	std::vector <std::vector<HistoryStatus>> history_status_list;	// ��¼��ʷ������Ϣ
	Timer timer_record_status;										// ��¼��ʱ����
	Timer timer_play_history_record;								// ��ʷ�ط�
	int idx_status = 0;												// ״̬����
	IMAGE* background = nullptr;									// ����ͼ
	bool is_reverse = false;
	bool is_record = true;
	bool is_enable = true;
	float progress = 0.0f;											// ����[0, 1]

private:
	ReverseTimeManager();
	~ReverseTimeManager();

public:
	static ReverseTimeManager* instance();

	void on_update(float delta);
	void on_render();
	void reverse_time();

	void set_enable(bool flag)
	{
		is_enable = true;
	}
	void set_record(bool flag)
	{
		is_record = flag;
	}

	bool check_over() const
	{
		return !is_reverse;
	}

	void clear()
	{
		is_reverse = false;
		progress = 0.0f;
		history_status_list.clear();
	}

	void skip();

	void set_background(IMAGE* img)
	{
		background = img;
	}

private:
	void capture_current_status();

	void update_play_speed(float delta);

	void render_background();
};

#endif // REVERSE_TIME_VERSION_2