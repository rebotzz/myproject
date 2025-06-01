#pragma once

enum class EnemyType
{
	Slime,								// 史莱姆
	KingSlime,							// 皇家史莱姆
	Skeleton,							// 骷髅兵
	Goblin,								// 哥布林
	GoblinPriest						// 哥布林祭祀
};

struct EnemyTemplate
{
public:
	int hp = 80;						// 生命上限
	float speed = 0.6;					// 移动速度(单位：瓦片格子)
	float damage = 1;					// 伤害
	float reward_ratio = 0.5;			// 掉金币概率
	float recover_interval = 10;		// (如果有)恢复技能cd
	float recover_range = -1;			// (如果有)恢复技能范围
	float recover_intensity = 25;		// (如果有)恢复技能强度
};
