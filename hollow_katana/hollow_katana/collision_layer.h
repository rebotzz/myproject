#pragma once

// ��ײ��
enum class CollisionLayer
{
	None	= 1,
	Enemy	= 1 << 1,		// ����
	Player	= 1 << 2,		// ���
	Scenery	= 1 << 3,		// ����
	Rebound	= 1 << 4,		// ��������
	Sword   = 1 << 5		// ƴ��
};

inline static CollisionLayer operator|(const CollisionLayer& layer_src, const CollisionLayer& layer_dst)
{
	return (CollisionLayer)((int)(layer_src) | (int)(layer_dst));
}

inline static CollisionLayer operator&(const CollisionLayer& layer_src, const CollisionLayer& layer_dst)
{
	return (CollisionLayer)((int)(layer_src) & (int)(layer_dst));
}