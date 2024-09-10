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