#pragma once
#include "bullet.h"
#include "animation.h"

extern Camera main_camera;
extern Atlas atlas_nut_explode;

// ½«¼á¹ûµÄ±¬Õ¨ÊÓÎª×Óµ¯
class NutExBullet : public Bullet
{
private:
	Animation _animation_explode;		// ×Óµ¯±¬Õ¨¶¯»­
	
public:
	NutExBullet()
	{
		// ±¬Õ¨Åö×²·¶Î§
		_size.x = 210;
		_size.y = 210;

		// ±¬Õ¨Í¼Æ¬³ß´ç
		_img_size.x = (float)atlas_nut_explode.get_image(0)->getwidth();
		_img_size.y = (float)atlas_nut_explode.get_image(0)->getheight();

		_animation_explode.set_atlas(&atlas_nut_explode);
		_animation_explode.set_interval(75);
		_animation_explode.set_loop(false);
		_animation_explode.set_callback([&]() 
			{ 
				// ¶ÌÔÝµÄ±¬Õ¨,ºÜ¿ìÊ§Ð§
				_valid = false;
				_can_remove = true; 
			}
		);

		_render_offset.x = _position.x + (_size.x - _img_size.x) / 2;
		_render_offset.y = _position.y + (_size.y - _img_size.y) / 2;
	}

	~NutExBullet()
	{}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		// ±¬Õ¨,Åö×²¼ì²â¾ØÐÎÊÇ·ñÏà½»
		bool is_x_collide = (max(_position.x + _size.x, position.x + size.x) - min(_position.x, position.x))
			< (size.x + _size.x);
		bool is_y_collide = (max(_position.y + _size.y, position.y + size.y) - min(_position.y, position.y))
			< (size.y + _size.y);

		return is_x_collide && is_y_collide;
	}

	virtual void on_update(int interval_ms) override
	{
		_animation_explode.on_update(interval_ms);

		if (_valid)
			main_camera.shake(20, 100);

		if (check_if_exceeds_screen())
			_can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const override
	{
		_animation_explode.on_draw(camera, (int)(_position.x + _render_offset.x), (int)(_position.y + _render_offset.y));

		Bullet::on_draw(camera);

		// ±¬Õ¨Í¼Æ¬±ß¿ò
		if (is_debug)
		{
			setlinecolor(RGB(255, 255, 51));
			rectangle(camera, (int)(_position.x + _render_offset.x), (int)(_position.y + _render_offset.y), 
				(int)(_position.x + _render_offset.x + _img_size.x), (int)(_position.y + _render_offset.y + _img_size.y));
		}
	}

};
