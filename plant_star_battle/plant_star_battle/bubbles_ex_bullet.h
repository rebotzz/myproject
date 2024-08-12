#pragma once
#include "bullet.h"
#include "animation.h"

extern Atlas atlas_gloomshroom_bubbles_ex;

// ������ı�ը��Ϊ�ӵ�
class BubblesExBullet : public Bullet
{
private:
	Animation _animation_bubbles_ex;		// �ӵ���ը����

public:
	BubblesExBullet()
	{
		// ��ը��ײ��Χ
		_size.x = 384;
		_size.y = 384;

		_animation_bubbles_ex.set_atlas(&atlas_gloomshroom_bubbles_ex);
		_animation_bubbles_ex.set_interval(75);
		_animation_bubbles_ex.set_loop(false);
		_animation_bubbles_ex.set_callback([&]()
			{
				// ���ݵı�ը,�ܿ�ʧЧ
				_valid = false;
				_can_remove = true;
			}
		);
	}

	~BubblesExBullet()
	{}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		// ���ݱ�ը,��ײ�������Ƿ��ཻ
		return check_collision_rect(position, size);
	}

	virtual void on_update(int interval_ms) override
	{
		_animation_bubbles_ex.on_update(interval_ms);

		if (_valid)
			main_camera.shake(10, 100);

		if (check_if_exceeds_screen())
			_can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const override
	{
		_animation_bubbles_ex.on_draw(camera, (int)(_position.x + _render_offset.x), (int)(_position.y + _render_offset.y));

		Bullet::on_draw(camera);
	}

};
