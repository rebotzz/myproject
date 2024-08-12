#pragma once
#include "bullet.h"
#include "animation.h"

extern Atlas atlas_gloomshroom_bubbles_ex;

// 将坚果的爆炸视为子弹
class BubblesExBullet : public Bullet
{
private:
	Animation _animation_bubbles_ex;		// 子弹爆炸动画

public:
	BubblesExBullet()
	{
		// 爆炸碰撞范围
		_size.x = 384;
		_size.y = 384;

		_animation_bubbles_ex.set_atlas(&atlas_gloomshroom_bubbles_ex);
		_animation_bubbles_ex.set_interval(75);
		_animation_bubbles_ex.set_loop(false);
		_animation_bubbles_ex.set_callback([&]()
			{
				// 短暂的爆炸,很快失效
				_valid = false;
				_can_remove = true;
			}
		);
	}

	~BubblesExBullet()
	{}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		// 泡泡爆炸,碰撞检测矩形是否相交
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
