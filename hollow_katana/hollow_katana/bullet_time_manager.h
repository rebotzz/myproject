#pragma once
#include <easyx.h>

// �ӵ�ʱ����: ������,������ʵʱ��,������Ϸʱ��
class BulletTimeManager
{
public:
	enum class Status
	{
		Enter, Exit
	};

private:
	static BulletTimeManager* manager;

private:
	const float SPEED_PROGRESS = 2.0f;					// �����ӵ�ʱ���ٶ�
	const float DST_DELTA_FACTOR = 0.35f;				// �����ӵ�ʱ�����ϵ��
	const float DST_COLOR_FACTOR = 0.35f;				// �����ӵ�ʱ��ȫ���䰵ϵ��
	float progress = 0.0f;								// ����[0, 1]
	Status status = Status::Exit;

private:
	BulletTimeManager() = default;
	~BulletTimeManager() = default;

public:
	static BulletTimeManager* instance();
	void set_status(Status status);

	// �������ź��ʱ��
	float on_update(float delta);

	// ȫ���䰵�ĺ�����Ч
	void post_progress();

private:
	// ���Բ�ֵ
	float lerp(float start, float end, float progress);
};
