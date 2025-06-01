#pragma once

#include "tower.h"

class TowerArcher : public Tower
{
public:
	TowerArcher(const Vector2& position_);
	~TowerArcher() = default;
};

TowerArcher::TowerArcher(const Vector2& position_)
	:Tower(TowerType::Archer, position_)
{
	// 动画初始化
	anim_idle_up.set_interval(0.3);
	anim_idle_down.set_interval(0.3);
	anim_idle_left.set_interval(0.3);
	anim_idle_right.set_interval(0.3);
	anim_attack_up.set_interval(0.2);
	anim_attack_down.set_interval(0.2);
	anim_attack_left.set_interval(0.2);
	anim_attack_right.set_interval(0.2);

	anim_idle_up.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerArcher), TILE_SIZE, TILE_SIZE, { 3,4 });
	anim_idle_down.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerArcher), TILE_SIZE, TILE_SIZE, { 0,1 });	// debug: 下标从0开始，不然裁剪空白区域导致闪烁
	anim_idle_left.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerArcher), TILE_SIZE, TILE_SIZE, { 6,7 });
	anim_idle_right.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerArcher), TILE_SIZE, TILE_SIZE, { 9,10 });

	anim_attack_up.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerArcher), TILE_SIZE, TILE_SIZE, { 15,16,17 });
	anim_attack_down.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerArcher), TILE_SIZE, TILE_SIZE, { 12,13,14 });
	anim_attack_left.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerArcher), TILE_SIZE, TILE_SIZE, { 18,19,20 });
	anim_attack_right.set_frame(ResMgr::instance()->find_texture(ResID::Tex_TowerArcher), TILE_SIZE, TILE_SIZE, { 21,22,23 });
}
