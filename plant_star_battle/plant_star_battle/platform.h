#pragma once
#include <easyx.h>
#include "camera.h"

extern bool is_debug;

// ƽ̨��
class Platform
{
public:
	// ������ײ���γ���Ϊһ����
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
			// ������ײ�����
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)_shape.left, (int)_shape.y, (int)_shape.right, (int)_shape.y);
		}
	}
};