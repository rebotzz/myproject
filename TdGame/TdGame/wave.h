#pragma once

#include "enemy_template.h"
#include <vector>

// ���˲�������
struct Wave
{
public:
	// ��������ʱ��
	struct SpawnEvent
	{
		float interval = 0;						// ���ɼ��
		int point = 0;							// ������
		EnemyType enemy = EnemyType::Slime;		// ����
	};
	typedef std::vector<SpawnEvent> SpawnList;

public:
	int rewards = 100;							// ���ν�������
	float interval = 3;							// ���μ��
	SpawnList spawn_list;						// �����е�����������
};

typedef std::vector<Wave> WaveList;