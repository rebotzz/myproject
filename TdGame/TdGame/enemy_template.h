#pragma once

enum class EnemyType
{
	Slime,								// ʷ��ķ
	KingSlime,							// �ʼ�ʷ��ķ
	Skeleton,							// ���ñ�
	Goblin,								// �粼��
	GoblinPriest						// �粼�ּ���
};

struct EnemyTemplate
{
public:
	int hp = 80;						// ��������
	float speed = 0.6;					// �ƶ��ٶ�(��λ����Ƭ����)
	float damage = 1;					// �˺�
	float reward_ratio = 0.5;			// ����Ҹ���
	float recover_interval = 10;		// (�����)�ָ�����cd
	float recover_range = -1;			// (�����)�ָ����ܷ�Χ
	float recover_intensity = 25;		// (�����)�ָ�����ǿ��
};
