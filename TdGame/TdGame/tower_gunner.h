#pragma once

#include "tower.h"

class TowerGunner : public Tower
{
public:
	TowerGunner(const Vector2& position_);
	~TowerGunner() = default;
};

TowerGunner::TowerGunner(const Vector2& position_)
	:Tower(TowerType::Gunner, position_)
{
	// ¶¯»­³õÊ¼»¯
	anim_idle_up.set_interval(0.3);
	anim_idle_down.set_interval(0.3);
	anim_idle_left.set_interval(0.3);
	anim_idle_right.set_interval(0.3);
	anim_attack_up.set_interval(0.2);
	anim_attack_down.set_interval(0.2);
	anim_attack_left.set_interval(0.2);
	anim_attack_right.set_interval(0.2);

	anim_idle_up.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerGunner), TILE_SIZE, TILE_SIZE, { 4,5 });
	anim_idle_down.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerGunner), TILE_SIZE, TILE_SIZE, { 0, 1 });
	anim_idle_left.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerGunner), TILE_SIZE, TILE_SIZE, { 12,13 });
	anim_idle_right.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerGunner), TILE_SIZE, TILE_SIZE, { 8,9 });

	anim_attack_up.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerGunner), TILE_SIZE, TILE_SIZE, { 20,21,22,23 });
	anim_attack_down.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerGunner), TILE_SIZE, TILE_SIZE, { 16,17,18,19 });
	anim_attack_left.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerGunner), TILE_SIZE, TILE_SIZE, { 28,29,30,31 });
	anim_attack_right.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerGunner), TILE_SIZE, TILE_SIZE, { 24, 25, 26, 27 });
}
