#pragma once
#include <vector>
#include "vector2.h"

class Path
{
private:
	float total_len = 0.0f;					// 路径总长度
	std::vector<Vector2> point_list;		// 每一段路径端点
	std::vector<float> segment_len_list;	// 每一段路径长度

public:
	Path(const std::vector<Vector2>& point_list)
	{
		this->point_list = point_list;
		for (int i = 1; i < point_list.size(); ++i)
		{
			float segment_len = (point_list[i] - point_list[i - 1]).length();
			total_len += segment_len;
			segment_len_list.push_back(segment_len);
		}
	}

	~Path() = default;

	Vector2 get_position_at_progress(float progress) const
	{
		if (progress <= 0) return point_list.front();
		if (progress >= 1) return point_list.back();

		float target_len = total_len * progress;
		float accumulated_len = 0.0f;
		for (int i = 1; i < point_list.size(); ++i)
		{
			accumulated_len += segment_len_list[i - 1];
			if (accumulated_len >= target_len)
			{
				float segment_progress = (target_len - (accumulated_len - segment_len_list[i - 1])) / segment_len_list[i - 1];
				return point_list[i - 1] + (point_list[i] - point_list[i - 1]) * segment_progress;
			}
		}
		return point_list.back();
	}
};