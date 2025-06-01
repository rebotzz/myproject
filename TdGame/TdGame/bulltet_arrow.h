#pragma once

#include "bullet.h"
#include "resources_manager.h"

class BulletArrow : public Bullet
{
public:
	BulletArrow(const Vector2& direction, const Vector2& init_position);
	~BulletArrow() = default;
protected:
	void play_hit_sound() override;

private:
	float speed = 6.5f;					// 移动速度，单位格子/秒
};

BulletArrow::BulletArrow(const Vector2& direction, const Vector2& init_position)
{
	anim.set_frame(ResMgr::instance()->find_texture(ResID::Tex_BulletArrow), TILE_SIZE, TILE_SIZE, { 0,1 });
	anim.set_loop(true);
	anim.set_interval(0.2);
	anim.set_angle(direction.angle());
	set_velocity(direction.normalize() * speed * TILE_SIZE);
	set_position(init_position);


	switch (rand() % 2)
	{
	case 0:
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_ArrowFire_1), 0);
		break;
	case 1:
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_ArrowFire_2), 0);
		break;
	}
}
void BulletArrow::play_hit_sound()
{
	switch (rand() % 3)
	{
	case 0:
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_ArrowHit_1), 0);
		break;
	case 1:
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_ArrowHit_2), 0);
		break;
	case 3:
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_ArrowHit_3), 0);
		break;
	}
}
