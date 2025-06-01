#pragma once

#include "tower.h"

class TowerAxeman : public Tower
{
public:
	TowerAxeman(const Vector2& position_);
	~TowerAxeman() = default;
};

TowerAxeman::TowerAxeman(const Vector2& position_)
	:Tower(TowerType::Axeman, position_)
{
	// 动画初始化
	anim_idle_up.set_interval(0.3);
	anim_idle_down.set_interval(0.3);
	anim_idle_left.set_interval(0.3);
	anim_idle_right.set_interval(0.3);			// todo：按照攻击间隔修改攻击动画帧间隔
	anim_attack_up.set_interval(0.45);
	anim_attack_down.set_interval(0.45);
	anim_attack_left.set_interval(0.45);
	anim_attack_right.set_interval(0.45);

	anim_idle_up.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerAxeman), TILE_SIZE, TILE_SIZE, { 3,4 });
	anim_idle_down.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerAxeman), TILE_SIZE, TILE_SIZE, { 0,1 });
	anim_idle_left.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerAxeman), TILE_SIZE, TILE_SIZE, { 9,10 });
	anim_idle_right.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerAxeman), TILE_SIZE, TILE_SIZE, { 6, 7 });

	anim_attack_up.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerAxeman), TILE_SIZE, TILE_SIZE, { 15,16,17 });
	anim_attack_down.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerAxeman), TILE_SIZE, TILE_SIZE, { 12,13,14 });
	anim_attack_left.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerAxeman), TILE_SIZE, TILE_SIZE, { 21,22,23 });
	anim_attack_right.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerAxeman), TILE_SIZE, TILE_SIZE, { 18, 19, 20 });
}
