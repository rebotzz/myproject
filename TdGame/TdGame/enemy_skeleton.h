#pragma once

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class EnemySkeleton : public Enemy
{
public:
	EnemySkeleton();
	~EnemySkeleton() = default;

};

EnemySkeleton::EnemySkeleton()
{
	// 基础属性
	static ConfigMgr* config = ConfigMgr::instance();
	attribute = config->enemy_skeleton_template;
	cur_hp = attribute.hp;

	// 动画初始化
	anim_up.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemySkeleton), TILE_SIZE, TILE_SIZE, { 5,6,7,8,9 });
	anim_down.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemySkeleton), TILE_SIZE, TILE_SIZE, { 0,1,2,3,4 });
	anim_left.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemySkeleton), TILE_SIZE, TILE_SIZE, { 15,16,17,18,19 });
	anim_right.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemySkeleton), TILE_SIZE, TILE_SIZE, { 10,11,12,13,14 });

	anim_up_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemySkeletonSketch), TILE_SIZE, TILE_SIZE, { 5,6,7,8,9 });
	anim_down_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemySkeletonSketch), TILE_SIZE, TILE_SIZE, { 0,1,2,3,4 });
	anim_left_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemySkeletonSketch), TILE_SIZE, TILE_SIZE, { 15,16,17,18,19 });
	anim_right_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemySkeletonSketch), TILE_SIZE, TILE_SIZE, { 10,11,12,13,14 });
}
