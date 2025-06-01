#pragma once

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class EnemyKingSlime : public Enemy
{
public:
	EnemyKingSlime();
	~EnemyKingSlime() = default;

};

EnemyKingSlime::EnemyKingSlime()
{
	// 基础属性
	static ConfigMgr* config = ConfigMgr::instance();
	attribute = config->enemy_king_slime_template;
	cur_hp = attribute.hp;
	hurt_box->set_size({ TILE_SIZE * 0.5, TILE_SIZE * 0.5 });

	// 动画初始化
	anim_up.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyKingSlime), TILE_SIZE, TILE_SIZE, { 18,19,20,21,22,23 });
	anim_down.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyKingSlime), TILE_SIZE, TILE_SIZE, { 0,1,2,3,4,5 });
	anim_left.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyKingSlime), TILE_SIZE, TILE_SIZE, { 6, 7, 8, 9, 10, 11 });
	anim_right.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyKingSlime), TILE_SIZE, TILE_SIZE, { 12,13,14,15,16,17 });

	anim_up_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyKingSlimeSketch), TILE_SIZE, TILE_SIZE, { 18,19,20,21,22,23 });
	anim_down_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyKingSlimeSketch), TILE_SIZE, TILE_SIZE, { 0,1,2,3,4,5 });
	anim_left_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyKingSlimeSketch), TILE_SIZE, TILE_SIZE, { 6, 7, 8, 9, 10, 11 });
	anim_right_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyKingSlimeSketch), TILE_SIZE, TILE_SIZE, { 12,13,14,15,16,17 });
}
