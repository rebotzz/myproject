#pragma once
#include <vector>

#define TILE_SIZE 48		// һ����Ƭ��ռ���ش�С 48 x 48

// ��ͼ������λ����Ƭ
struct Tile
{
	// �����н�����,��map.csv��Ӧ
	enum class Direction
	{
		None = 0,
		Up, 
		Down, 
		Left, 
		Right
	};

	int terrian = 0;						// �ײ���Ƭ��ͼ���
	int decoration = -1;					// װ�β���ͼ���;-1��ʾû��
	Direction direction = Direction::None;	// ǰ���ķ���
	int speacial_flag = -1;					// -1:ʲôҲû�У�0:home��1��2��3��4��5:��ʾ�������ɵ�
	bool has_tower = false;					// �Ƿ�����
};

typedef std::vector<std::vector<Tile>> TileMap;