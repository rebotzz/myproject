#pragma once
#include "bullet.h"
#include "animation.h"

extern Atlas atlas_gloomshroom_bubbles;

// ½«ÓÇÓô¹½µÄÅÝÅÝÊÓÎª×Óµ¯
class BubblesBullet : public Bullet
{
private:
	Animation _animation_bubbles;		// ×Óµ¯ÅÝÅÝ¶¯»­

public:
	BubblesBullet()
	{
		// ÅÝÅÝÅö×²·¶Î§
		_size.x = 192;
		_size.y = 192;

		_animation_bubbles.set_atlas(&atlas_gloomshroom_bubbles);
		_animation_bubbles.set_interval(75);
		_animation_bubbles.set_loop(false);
		_animation_bubbles.set_callback([&]()
			{
				// ¶ÌÔÝµÄÅÝÅÝ,ºÜ¿ìÊ§Ð§
				_valid = false;
				_can_remove = true;
			}
		);
	}

	~BubblesBullet()
	{}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		// ÅÝÅÝ±¬Õ¨,Åö×²¼ì²â¾ØÐÎÊÇ·ñÏà½»
		return check_collision_rect(position, size);
	}

	virtual void on_update(int interval_ms) override
	{
		_animation_bubbles.on_update(interval_ms);

		if (check_if_exceeds_screen())
			_can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const override
	{
		_animation_bubbles.on_draw(camera, (int)(_position.x + _render_offset.x), (int)(_position.y + _render_offset.y));

		Bullet::on_draw(camera);
	}

};
