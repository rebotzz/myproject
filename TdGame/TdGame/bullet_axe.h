#pragma once

#include "bullet.h"
#include "resources_manager.h"

class BulletAxe : public Bullet
{
public:
	BulletAxe(const Vector2& direction, const Vector2& init_position);
	~BulletAxe() = default;
protected:
	void play_hit_sound() override;

private:
	float speed = 6.0f;					// 移动速度，单位格子/秒
};

BulletAxe::BulletAxe(const Vector2& direction, const Vector2& init_position)
{
	anim.set_frame(ResMgr::instance()->find_texture(ResID::Tex_BulletAxe), TILE_SIZE, TILE_SIZE, { 0,1,2,3,4,5,6,7 });
	anim.set_loop(true);
	anim.set_interval(0.1);
	set_velocity(direction.normalize() * speed * TILE_SIZE);
	set_position(init_position);
	slow_debuff = true;
	slow_scale = 0.4;

	Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_AxeFire), 0);
}
void BulletAxe::play_hit_sound()
{
	switch (rand() % 3)
	{
	case 0:
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_AxeHit_1), 0);
		break;
	case 1:
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_AxeHit_2), 0);
		break;
	case 3:
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_AxeHit_3), 0);
		break;
	}
}
