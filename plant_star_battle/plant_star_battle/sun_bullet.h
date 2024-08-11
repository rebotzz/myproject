#pragma once
#include "bullet.h"
#include "animation.h"

extern Atlas atlas_sunflower_sun_idle;
extern Atlas atlas_sunflower_sun_explode;
extern Camera main_camera;

// 日光炸弹类
class SunBullet : public Bullet
{
private:
	Animation _animation_sum_idle;		// 日光炸弹闲置动画
	Animation _animation_sum_explode;	// 日光炸弹爆炸动画
	float _gravity = 1.6e-3f;			// 日光炸弹重力加速度
	Vector2 _explode_render_offset;		// 爆炸动画的偏移量,像素大小:爆炸动画 > 日光炸弹
public:
	SunBullet()
	{
		_size.x = 96;
		_size.y = 96;
		_damage = 20;

		_animation_sum_idle.set_atlas(&atlas_sunflower_sun_idle);
		_animation_sum_idle.set_interval(50);
		_animation_sum_idle.set_loop(true);

		_animation_sum_explode.set_atlas(&atlas_sunflower_sun_explode);
		_animation_sum_explode.set_interval(50);
		_animation_sum_explode.set_loop(false);
		_animation_sum_explode.set_callback([&]() { _can_remove = true; });

		IMAGE* img_idle = _animation_sum_idle.get_frame();
		IMAGE* img_explode = _animation_sum_explode.get_frame();
		_explode_render_offset.x = (img_idle->getwidth() - img_explode->getwidth()) / 2.0f;
		_explode_render_offset.y = (img_idle->getheight() - img_explode->getheight()) / 2.0f;
	}


	virtual void on_collide() override
	{
		Bullet::on_collide();

		main_camera.shake(5, 250);

		mciSendString(L"play sun_explode from 0", nullptr, 0, nullptr);
	}

	virtual void on_update(int interval_ms) override
	{
		if (_valid)
		{
			_velocity.y += _gravity * (float)interval_ms;
			_position += _velocity * (float)interval_ms;
			_animation_sum_idle.on_update(interval_ms);
		}
		else
		{
			_animation_sum_explode.on_update(interval_ms);
		}


		if (check_if_exceeds_screen())
			_can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const override
	{
		if (_valid)
			_animation_sum_idle.on_draw(camera, (int)_position.x, (int)_position.y);
		else
		{
			_animation_sum_explode.on_draw(camera,
				(int)(_position.x + _explode_render_offset.x),
				(int)(_position.y + _explode_render_offset.y));
		}

		Bullet::on_draw(camera);
	}
};