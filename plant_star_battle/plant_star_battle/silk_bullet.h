#pragma once
#include "bullet.h"
#include "animation.h"

extern Camera main_camera;
extern Atlas atlas_hornet_silk;

// 将大黄蜂丝线的视为子弹
class SilkBullet : public Bullet
{
private:
	Animation _animation_silk;		// 子弹爆炸动画
	int _valid_count = 3;			// 生效次数

public:
	SilkBullet()
	{
		// 爆炸碰撞范围
		_size.x = 250;
		_size.y = 250;
		_damage = 10;

		// 爆炸图片尺寸
		_img_size.x = (float)atlas_hornet_silk.get_image(0)->getwidth();
		_img_size.y = (float)atlas_hornet_silk.get_image(0)->getheight();


		// 播放两次动画
		static Atlas atlas_hornet_silk_2 = atlas_hornet_silk;
		static bool init = true;
		if (init)
		{
			init = false;
			for (int i = 0; i < atlas_hornet_silk.get_size(); ++i)
				atlas_hornet_silk_2.add_image(*atlas_hornet_silk.get_image(i));
		}

		_animation_silk.set_atlas(&atlas_hornet_silk_2);
		_animation_silk.set_interval(75);
		_animation_silk.set_loop(false);
		_animation_silk.set_callback([&]()
			{
				// 短暂的爆炸,很快失效
				_valid = false;
				_can_remove = true;
			}
		);

		_render_offset.x = _position.x + (_size.x - _img_size.x) / 2;
		_render_offset.y = _position.y + (_size.y - _img_size.y) / 2;
	}

	virtual void set_valid(bool flag) override
	{
		if (flag == true)
			_valid = true;

		if (flag == false)
		{
			if (_valid_count > 0)
				_valid_count--;

			if (_valid_count == 0)
				_valid = false;
		}
	}

	~SilkBullet()
	{}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		// 爆炸,碰撞检测矩形是否相交
		bool is_x_collide = (max(_position.x + _size.x, position.x + size.x) - min(_position.x, position.x))
			< (size.x + _size.x);
		bool is_y_collide = (max(_position.y + _size.y, position.y + size.y) - min(_position.y, position.y))
			< (size.y + _size.y);

		return is_x_collide && is_y_collide;
	}

	virtual void on_update(int interval_ms) override
	{
		_animation_silk.on_update(interval_ms);

		if (_valid)
			main_camera.shake(3, 100);

		if (check_if_exceeds_screen())
			_can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const override
	{
		_animation_silk.on_draw(camera, (int)(_position.x + _render_offset.x), (int)(_position.y + _render_offset.y));

		Bullet::on_draw(camera);

		// 图片边框
		if (is_debug)
		{
			setlinecolor(RGB(255, 255, 51));
			rectangle(camera, (int)(_position.x + _render_offset.x), (int)(_position.y + _render_offset.y),
				(int)(_position.x + _render_offset.x + _img_size.x), (int)(_position.y + _render_offset.y + _img_size.y));
		}
	}

};
