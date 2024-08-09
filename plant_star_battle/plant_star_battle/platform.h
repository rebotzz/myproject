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
	}
};