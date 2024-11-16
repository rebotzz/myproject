#pragma once

// 碰撞层
enum class CollisionLayer
{
	None	 = 1,
	Enemy	 = 1 << 1,		// 敌人
	Player	 = 1 << 2,		// 玩家
	Scenery  = 1 << 3,		// 场景
	Rebound	 = 1 << 4,		// 反弹物体
	Sword    = 1 << 5,		// 拼刀
	Interact = 1 << 6		// 交互道具
};

inline static CollisionLayer operator|(const CollisionLayer& layer_src, const CollisionLayer& layer_dst)
{
	return (CollisionLayer)((int)(layer_src) | (int)(layer_dst));
}

inline static CollisionLayer operator&(const CollisionLayer& layer_src, const CollisionLayer& layer_dst)
{
	return (CollisionLayer)((int)(layer_src) & (int)(layer_dst));
}