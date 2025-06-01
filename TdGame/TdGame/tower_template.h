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
	// ��ͬ�ȼ���ֵ��ͬ����ʼ0�����9��
	std::array<float, 10> interval = { 0 };					// �������
	std::array<float, 10> damage = { 0 };					// �˺�
	std::array<float, 10> view_range = { 0 };				// ��Ұ(����)��Χ��(��λ:����)
	std::array<int, 10> cost = { 0 };						// ���컨��
	std::array<int, 9> upgrade_cost = { 0 };				// ��������
};