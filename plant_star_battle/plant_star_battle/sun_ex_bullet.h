#pragma once
#include "bullet.h"
#include "animation.h"

extern Atlas atlas_sunflower_sun_ex_idle;
extern Atlas atlas_sunflower_sun_ex_explode;
extern Camera main_camera;

// 大型日光炸弹类
class SunExBullet : public Bullet
{
private:
	Animation _animation_sum_ex_idle;		// 大型日光炸弹闲置动画
	Animation _animation_sum_ex_explode;	// 大型日光炸弹爆炸动画
	Vector2 _explode_render_offset;			// 爆炸动画的偏移量

public:
	SunExBullet()
	{
		// 感觉可以直接读取图片尺寸,不过手动调整可以改变碰撞体积
		_size.x = 288;
		_size.y = 288;
		_damage = 20;

		_animation_sum_ex_idle.set_atlas(&atlas_sunflower_sun_ex_idle);
		_animation_sum_ex_idle.set_interval(50);
		_animation_sum_ex_idle.set_loop(true);

		_animation_sum_ex_explode.set_atlas(&atlas_sunflower_sun_ex_explode);
		_animation_sum_ex_explode.set_interval(50);
		_animation_sum_ex_explode.set_loop(false);
		_animation_sum_ex_explode.set_callback([&]() { _can_remove = true; });

		IMAGE* img_idle = _animation_sum_ex_idle.get_frame();
		IMAGE* img_explode = _animation_sum_ex_explode.get_frame();
		_explode_render_offset.x = (img_idle->getwidth() - img_explode->getwidth()) / 2.0f;
		_explode_render_offset.y = (img_idle->getheight() - img_explode->getheight()) / 2.0f;
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		// 由于大型日光炸弹过大,碰撞检测矩形是否相交
		bool is_x_collide = (max(_position.x + _size.x, position.x + size.x) - min(_position.x, position.x))
			< (size.x + _size.x);
		bool is_y_collide = (max(_position.y + _size.y, position.y + size.y) - min(_position.y, position.y))
			< (size.y + _size.y);

		return is_x_collide && is_y_collide;
	}

	virtual void on_collide() override
	{
		Bullet::on_collide();

		main_camera.shake(20, 350);

		mciSendString(L"play sun_explode_ex from 0", nullptr, 0, nullptr);
	}

	virtual void on_update(int interval_ms) override
	{
		if (_valid)
		{
			_position+= _velocity * (float)interval_ms;
			_animation_sum_ex_idle.on_update(interval_ms);
		}
		else
		{
			_animation_sum_ex_explode.on_update(interval_ms);
		}

		if (check_if_exceeds_screen())
			_can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const override
	{
		if (_valid)
			_animation_sum_ex_idle.on_draw(camera, (int)_position.x, (int)_position.y);
		else
		{
			_animation_sum_ex_explode.on_draw(camera,
				(int)(_position.x + _explode_render_offset.x),
				(int)(_position.y + _explode_render_offset.y));
		}

		Bullet::on_draw(camera);
	}

};