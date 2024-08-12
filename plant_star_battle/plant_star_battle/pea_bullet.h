#pragma once
#include "bullet.h"
#include "animation.h"

extern IMAGE img_pea_bullet;
extern Atlas atlas_pea_bullet_break;

// �㶹�ӵ���
class PeaBullet : public Bullet
{
private:
	IMAGE* _img;					// �ӵ�ͼƬ
	Animation _animation_break;		// �ӵ����鶯��

public:
	PeaBullet()
	{
		_size.x = 64;
		_size.y = 64;
		_damage = 10;

		_img = &img_pea_bullet;
		_animation_break.set_atlas(&atlas_pea_bullet_break);
		_animation_break.set_interval(100);
		_animation_break.set_loop(false);
		_animation_break.set_callback([&]() { _can_remove = true; });
	}
	~PeaBullet() {}

	virtual void on_collide()
	{
		Bullet::on_collide();

		// ������������ӵ���ײ��Ч,�����ʸ�
		switch (rand() % 3)
		{
		case 0:
			mciSendString(L"play pea_break_1 from 0", nullptr, 0, nullptr);
			break;
		case 1:
			mciSendString(L"play pea_break_2 from 0", nullptr, 0, nullptr);
			break;
		case 2:
			mciSendString(L"play pea_break_3 from 0", nullptr, 0, nullptr);
			break;
		}
	}

	virtual void on_update(int interval_ms)
	{
		_position += _velocity * (float)interval_ms;

		if (!_valid)
			_animation_break.on_update(interval_ms);

		if (check_if_exceeds_screen())
			_can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const
	{
		if (_valid)
			putimage_alpha(camera, (int)_position.x, (int)_position.y, _img);
		else
			_animation_break.on_draw(camera, (int)_position.x, (int)_position.y);

		Bullet::on_draw(camera);
	}

};