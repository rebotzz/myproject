#pragma once
#include <easyx.h>
#include "camera.h"

extern bool is_debug;

// 平台类
class Platform
{
public:
	// 这里碰撞外形抽象为一条线
	struct CollisionShape
	{
		float y = 0;
		float left = 0, right = 0;
	};
public:
	CollisionShape _shape;
	IMAGE* _img = nullptr;
	POINT _render_position = { 0, 0 };

public:
	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, _render_position.x, _render_position.y, _img);

		if (is_debug)
		{
			// 绘制碰撞检测线
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)_shape.left, (int)_shape.y, (int)_shape.right, (int)_shape.y);
		}
	}
};