#pragma once
#include <vector>

#define TILE_SIZE 48		// 一个瓦片所占像素大小 48 x 48

// 地图基本单位：瓦片
struct Tile
{
	// 敌人行进方向,与map.csv对应
	enum class Direction
	{
		None = 0,
		Up, 
		Down, 
		Left, 
		Right
	};

	int terrian = 0;						// 底层瓦片贴图序号
	int decoration = -1;					// 装饰层贴图序号;-1表示没有
	Direction direction = Direction::None;	// 前进的方向
	int speacial_flag = -1;					// -1:什么也没有；0:home；1、2、3、4、5:表示敌人生成点
	bool has_tower = false;					// 是否有塔
};

typedef std::vector<std::vector<Tile>> TileMap;