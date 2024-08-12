#pragma once
#include "bullet.h"
#include "animation.h"
#include "buff_id.h"

// ����buff
extern Atlas atlas_buff_box_blue;
extern Atlas atlas_buff_box_grey;
extern Atlas atlas_buff_box_red;
extern Atlas atlas_buff_box_yellow;

// �������buff,����ʹ���ӵ���ģ��ʵ��	(�ӵ�������ٳ���һ���ƶ�������ײ��)
class Buff : public Bullet
{
protected:
	Animation _animation_buff;
	BuffID _buff_id = BuffID::INVALID;

public:
	Buff()
	{
		_velocity.y = 0.1f;
		_damage = 0;

		_position.y = 50;	// ��������Ļ��,��can_remove
		_position.x = (float)(300 + rand() % (getwidth() - 500));
	}

	void set_atlas(Atlas* atlas)
	{
		_animation_buff.set_atlas(atlas);
		_animation_buff.set_interval(100);
		_animation_buff.set_loop(true);

		_size.x = (float)_animation_buff.get_frame()->getwidth();
		_size.y = (float)_animation_buff.get_frame()->getheight();
	}

	BuffID get_buff_id()
	{  
		return _buff_id;
	}

	virtual void on_collide() override
	{
		Bullet::on_collide();

		// ���Ż�ȡbuff��Ч
		mciSendString(L"play buff from 0", nullptr, 0, nullptr);
	}

	virtual void on_update(int interval_ms) override
	{
		if (_valid)
		{
			_position += _velocity * (float)interval_ms;
			_animation_buff.on_update(interval_ms);
		}
		else
			_can_remove = true;

		if (check_if_exceeds_screen())
			_can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const override
	{
		if (_valid)
			_animation_buff.on_draw(camera, (int)_position.x, (int)_position.y);

		Bullet::on_draw(camera);
	}
};


class BuffRecoveryHP : public Buff
{
public:
	BuffRecoveryHP()
	{
		set_atlas(&atlas_buff_box_red);
		_buff_id = BuffID::RECOVERY_HP;
	}
};

class BuffRecoveryMP : public Buff
{
public:
	BuffRecoveryMP()
	{
		set_atlas(&atlas_buff_box_blue);
		_buff_id = BuffID::RECOVERY_MP;
	}
};

class BuffInvisible : public Buff
{
public:
	BuffInvisible()
	{
		set_atlas(&atlas_buff_box_grey);
		_buff_id = BuffID::INVISIBLE;
	}
};

class BuffHurry : public Buff
{
public:
	BuffHurry()
	{
		set_atlas(&atlas_buff_box_yellow);
		_buff_id = BuffID::HURRY;
	}
};