#pragma once

#include "enemy_template.h"
#include <vector>

// 敌人波次描述
struct Wave
{
public:
	// 敌人生成时间
	struct SpawnEvent
	{
		float interval = 0;						// 生成间隔
		int point = 0;							// 出生点
		EnemyType enemy = EnemyType::Slime;		// 类型
	};
	typedef std::vector<SpawnEvent> SpawnList;

public:
	int rewards = 100;							// 波次结束奖励
	float interval = 3;							// 波次间隔
	SpawnList spawn_list;						// 波次中敌人生成序列
};

typedef std::vector<Wave> WaveList;