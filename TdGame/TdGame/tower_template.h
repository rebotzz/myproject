#pragma once
#include <array>

enum class TowerType
{
	Archer,
	Axeman,
	Gunner
};

struct TowerTemplate
{
public:
	// 不同等级数值不同，初始0，最高9级
	std::array<float, 10> interval = { 0 };					// 攻击间隔
	std::array<float, 10> damage = { 0 };					// 伤害
	std::array<float, 10> view_range = { 0 };				// 视野(攻击)范围，(单位:格子)
	std::array<int, 10> cost = { 0 };						// 建造花费
	std::array<int, 9> upgrade_cost = { 0 };				// 升级花费
};