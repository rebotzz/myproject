#pragma once

// Åö×²²ã
enum class CollisionLayer
{
	None		= 1,
	Enemy		= 1 << 1,
	Player		= 1 << 2,
	Scenery		= 1 << 3,
	Rebound		= 1 << 4

};

inline static CollisionLayer operator|(const CollisionLayer& layer_src, const CollisionLayer& layer_dst)
{
	return (CollisionLayer)((int)(layer_src) | (int)(layer_dst));
}