#pragma once
#include "bullet.h"
#include "animation.h"

extern Atlas atlas_hornet_sword_left;
extern Atlas atlas_hornet_sword_right;

// π«Ω£µØ¿‡
class SwordBullet : public Bullet
{
private:
	Animation _animation_sword;		// ∂Øª≠

public:
	SwordBullet(bool is_face_left = true)
	{
		_size.x = (float)atlas_hornet_sword_left.get_image(0)->getwidth();
		_size.y = (float)atlas_hornet_sword_left.get_image(0)->getheight();

		if(is_face_left)
			_animation_sword.set_atlas(&atlas_hornet_sword_left);
		else
			_animation_sword.set_atlas(&atlas_hornet_sword_right);

		_animation_sword.set_interval(100);
		_animation_sword.set_loop(true);
	}
	~SwordBullet() {}

	virtual void on_collide()
	{
		Bullet::on_collide();

		_can_remove = true;
		// “Ù–ß
	}

	virtual void on_update(int interval_ms)
	{
		if (_valid)
		{
			_position += _velocity * (float)interval_ms;
			_animation_sword.on_update(interval_ms);
		}

		if (check_if_exceeds_screen())
			_can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const
	{
		if (_valid)
			_animation_sword.on_draw(camera, (int)_position.x, (int)_position.y);

		Bullet::on_draw(camera);
	}

};