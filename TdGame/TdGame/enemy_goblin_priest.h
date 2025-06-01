#pragma once

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"


class EnemyGoblinPriest : public Enemy
{
public:
	EnemyGoblinPriest();
	~EnemyGoblinPriest();

	void on_update(float delta) override;

private:
	void recover_skill();

private:
	Timer timer_recover_skill;
	CollisionBox* recover_box = nullptr;
	bool is_recover = false;
};

EnemyGoblinPriest::EnemyGoblinPriest()
{
	// 基础属性
	static ConfigMgr* config = ConfigMgr::instance();
	attribute = config->enemy_goblin_priest_template;
	cur_hp = attribute.hp;

	// 动画初始化
	anim_up.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyGoblinPriest), TILE_SIZE, TILE_SIZE, { 5,6,7,8,9 });
	anim_down.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyGoblinPriest), TILE_SIZE, TILE_SIZE, { 0,1,2,3,4 });
	anim_left.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyGoblinPriest), TILE_SIZE, TILE_SIZE, { 15,16,17,18,19 });
	anim_right.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyGoblinPriest), TILE_SIZE, TILE_SIZE, { 10,11,12,13,14 });

	anim_up_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyGoblinPriestSketch), TILE_SIZE, TILE_SIZE, { 5,6,7,8,9 });
	anim_down_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyGoblinPriestSketch), TILE_SIZE, TILE_SIZE, { 0,1,2,3,4 });
	anim_left_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyGoblinPriestSketch), TILE_SIZE, TILE_SIZE, { 15,16,17,18,19 });
	anim_right_sketch.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EnemyGoblinPriestSketch), TILE_SIZE, TILE_SIZE, { 10,11,12,13,14 });

	timer_recover_skill.set_one_shot(false);
	timer_recover_skill.set_wait_time(attribute.recover_interval);
	timer_recover_skill.set_on_timeout([this]()
		{
			recover_skill();
		});

	recover_box = CollisionMgr::instance()->create_collision_box();
	recover_box->set_dst_layer(CollisionLayer::Enemy);
	recover_box->set_shape_mode(CollisionBox::ShapeMode::Circle);
	recover_box->set_radius(get_recover_range());
	recover_box->set_valid(false);
	recover_box->set_on_collide([this]()
		{
			Enemy* enemy = reinterpret_cast<Enemy*>(recover_box->get_on_collide_box()->get_user_data());
			if (!enemy || enemy->can_remove()) return;
			enemy->increase_hp(get_recover_intensity());
		});

}

EnemyGoblinPriest::~EnemyGoblinPriest()
{
	CollisionMgr::instance()->destroy_collision_box(recover_box);
}

void EnemyGoblinPriest::recover_skill()
{
	// 碰撞盒子
	increase_hp(get_recover_intensity());
	recover_box->set_valid(true);
	is_recover = true;
}

void EnemyGoblinPriest::on_update(float delta)
{
	Enemy::on_update(delta);

	 //保证先后顺序,timer_skill在后面：开启技能后，碰撞检测区域恢复生命，下一更新关闭恢复碰撞盒子
	if (is_recover)
	{
		is_recover = false;
		recover_box->set_valid(false);
	}
	timer_recover_skill.on_update(delta);
	recover_box->set_position(position);
}


