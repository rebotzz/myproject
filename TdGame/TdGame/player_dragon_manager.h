#pragma once

#include "singleton.h"
#include "player_template.h"
#include "resources_manager.h"
#include "config_manager.h"
#include "collision_manager.h"
#include "timer.h"
#include "vector2.h"
#include "enemy.h"

class PlayerDragonMgr : public Singleton<PlayerDragonMgr>
{
	friend class Singleton<PlayerDragonMgr>;
	enum class Direction
	{
		Up, Down, Left, Right
	};
public:
	PlayerDragonMgr()
	{
		// 基础属性
		attribute = ConfigMgr::instance()->player_template;
		position = { TILE_SIZE * 5, TILE_SIZE * 5 };
		// 攻击定时器
		timer_attack.set_one_shot(false);
		timer_attack.set_wait_time(attribute.normal_attack_interval);
		timer_attack.set_on_timeout([this]()
			{
				can_normal_attack = true;
			});
		timer_skill.set_one_shot(false);
		timer_skill.set_wait_time(0.1);
		timer_skill.set_on_timeout([this]()
			{	
				mp = std::min(100, mp + 3);
			});

		// 碰撞盒子初始化：动态修改轴向/纵向/位置
		attack_hit_box = CollisionMgr::instance()->create_collision_box();
		attack_hit_box->set_valid(false);
		attack_hit_box->set_dst_layer(CollisionLayer::Enemy);
		attack_hit_box->set_size({ 64 ,300 });
		attack_hit_box->set_on_collide([this]()
			{
				Enemy* enemy = reinterpret_cast<Enemy*>(attack_hit_box->get_on_collide_box()->get_user_data());
				if (!enemy || enemy->can_remove()) return;
				enemy->decrease_hp(attribute.normal_attack_damage);
			});
		skill_hit_box = CollisionMgr::instance()->create_collision_box();
		skill_hit_box->set_valid(false);
		skill_hit_box->set_dst_layer(CollisionLayer::Enemy);
		skill_hit_box->set_size({ 140 ,60 });
		skill_hit_box->set_on_collide([this]()
			{
				Enemy* enemy = reinterpret_cast<Enemy*>(skill_hit_box->get_on_collide_box()->get_user_data());
				if (!enemy || enemy->can_remove()) return;
				enemy->decrease_hp(attribute.skill_damage);
			});
		pick_box = CollisionMgr::instance()->create_collision_box();
		pick_box->set_dst_layer(CollisionLayer::Prop);
		pick_box->set_size({ 96 ,96 });		

		// 动画初始化
		// 玩家龙动画
		anim_up_idle.set_interval(0.1);
		anim_up_idle.set_loop(true);
		anim_up_idle.set_frame(ResMgr::instance()->find_texture(ResID::Tex_Player), 96, 96, { 4, 5,6,7 });

		anim_down_idle.set_interval(0.1);
		anim_down_idle.set_loop(true);
		anim_down_idle.set_frame(ResMgr::instance()->find_texture(ResID::Tex_Player), 96, 96, { 0,1,2,3 });

		anim_left_idle.set_interval(0.1);
		anim_left_idle.set_loop(true);
		anim_left_idle.set_frame(ResMgr::instance()->find_texture(ResID::Tex_Player), 96, 96, { 8,9,10,11 });

		anim_right_idle.set_interval(0.1);
		anim_right_idle.set_loop(true);
		anim_right_idle.set_frame(ResMgr::instance()->find_texture(ResID::Tex_Player), 96, 96, { 12,13,14,15 });

		anim_up_attack.set_interval(0.1);
		anim_up_attack.set_loop(true);
		anim_up_attack.set_frame(ResMgr::instance()->find_texture(ResID::Tex_Player), 96, 96, { 20,21 });

		anim_down_attack.set_interval(0.1);
		anim_down_attack.set_loop(true);
		anim_down_attack.set_frame(ResMgr::instance()->find_texture(ResID::Tex_Player), 96, 96, { 16,17 });

		anim_left_attack.set_interval(0.1);
		anim_left_attack.set_loop(true);
		anim_left_attack.set_frame(ResMgr::instance()->find_texture(ResID::Tex_Player), 96, 96, { 24,25 });

		anim_right_attack.set_interval(0.1);
		anim_right_attack.set_loop(true);
		anim_right_attack.set_frame(ResMgr::instance()->find_texture(ResID::Tex_Player), 96, 96, { 28,29 });
		cur_anim = &anim_left_idle;

		// 攻击特效动画
		anim_vfx_up_attack.set_interval(attribute.normal_attack_interval / 5.0);
		anim_vfx_up_attack.set_loop(false);
		anim_vfx_up_attack.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EffectFlashUp), 68, 300, { 0,1,2,3,4 });
		anim_vfx_up_attack.set_on_finished([this]() { is_attacking = false; });

		anim_vfx_down_attack.set_interval(attribute.normal_attack_interval / 5.0);
		anim_vfx_down_attack.set_loop(false);
		anim_vfx_down_attack.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EffectFlashDown), 68, 300, { 4,3,2,1,0 });
		anim_vfx_down_attack.set_on_finished([this]() { is_attacking = false; });

		anim_vfx_left_attack.set_interval(attribute.normal_attack_interval / 5.0);
		anim_vfx_left_attack.set_loop(false);
		anim_vfx_left_attack.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EffectFlashLeft), 300, 68, { 4,3,2,1,0 });
		anim_vfx_left_attack.set_on_finished([this]() { is_attacking = false; });

		anim_vfx_right_attack.set_interval(attribute.normal_attack_interval / 5.0);
		anim_vfx_right_attack.set_loop(false);
		anim_vfx_right_attack.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EffectFlashRight), 300, 68, { 0,1,2,3,4 });
		anim_vfx_right_attack.set_on_finished([this]() { is_attacking = false; });

		anim_vfx_up_skill.set_interval(0.075);
		anim_vfx_up_skill.set_loop(false);
		anim_vfx_up_skill.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EffectImpactUp), 140, 60, { 0,1,2,3,4 });
		anim_vfx_up_skill.set_on_finished([this]() { is_attacking = false; });

		anim_vfx_down_skill.set_interval(0.075);
		anim_vfx_down_skill.set_loop(false);
		anim_vfx_down_skill.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EffectImpactDown), 140, 60, { 4,3,2,1,0 });
		anim_vfx_down_skill.set_on_finished([this]() { is_attacking = false; });

		anim_vfx_left_skill.set_interval(0.075);
		anim_vfx_left_skill.set_loop(false);
		anim_vfx_left_skill.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EffectImpactLeft), 60, 140, { 4,3,2,1,0 });
		anim_vfx_left_skill.set_on_finished([this]() { is_attacking = false; });

		anim_vfx_right_skill.set_interval(0.075);
		anim_vfx_right_skill.set_loop(false);
		anim_vfx_right_skill.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EffectImpactRight), 60, 140, { 0,1,2,3,4 });
		anim_vfx_right_skill.set_on_finished([this]() { is_attacking = false; });
		cur_anim_vfx = &anim_vfx_left_attack;
	}

	~PlayerDragonMgr()
	{
		CollisionMgr::instance()->destroy_collision_box(attack_hit_box);
		CollisionMgr::instance()->destroy_collision_box(skill_hit_box);
		CollisionMgr::instance()->destroy_collision_box(pick_box);
	}

	void on_input(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_w: is_up_keydown = true; break;
			case SDLK_s: is_down_keydown = true; break;
			case SDLK_a: is_left_keydown = true; break;
			case SDLK_d: is_right_keydown = true; break;
			case SDLK_j: is_attack_keydown = true; break;
			case SDLK_k: is_skill_keydown = true; break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_w: is_up_keydown = false; break;
			case SDLK_s: is_down_keydown = false; break;
			case SDLK_a: is_left_keydown = false; break;
			case SDLK_d: is_right_keydown = false; break;
			case SDLK_j: is_attack_keydown = false; break;
			case SDLK_k: is_skill_keydown = false; break;
			}
			break;
		}
	}

	void on_update(float delta)
	{
		// 位置移动
		velocity.x = 0, velocity.y = 0;
		if (!is_attacking && is_up_keydown) velocity.y -= 1, direction = Direction::Up;
		if (!is_attacking && is_down_keydown) velocity.y += 1, direction = Direction::Down;
		if (!is_attacking && is_left_keydown) velocity.x -= 1, direction = Direction::Left;
		if (!is_attacking && is_right_keydown) velocity.x += 1, direction = Direction::Right;
		position += velocity.normalize() * attribute.speed * TILE_SIZE * delta;
		static int map_w = ConfigMgr::instance()->tile_map.empty() ? 0 : ConfigMgr::instance()->tile_map[0].size() * TILE_SIZE;
		static int map_h = ConfigMgr::instance()->tile_map.size() * TILE_SIZE;
		if (position.x > map_w - 96 / 2) position.x = map_w - 96 / 2;
		if(position.y > map_h - 96 / 2)  position.y = map_h - 96 / 2;
		if (position.x < 96 / 2)  position.x = 96 / 2;
		if(position.y < 96 / 2)  position.y = 96 / 2;
		pick_box->set_position(position);

		// 攻击
		timer_attack.on_update(delta);
		timer_skill.on_update(delta);
		if (is_attacking)	
		{	// 攻击一次结束后，关闭碰撞，避免"帧伤";（先关闭，后开启）
			attack_hit_box->set_valid(false);
			skill_hit_box->set_valid(false);
		}
		if (is_attack_keydown && !is_attacking && can_normal_attack) attack();
		if (is_skill_keydown && !is_attacking && mp == 100) skill();

		// 动画
		switch (direction)
		{
		case PlayerDragonMgr::Direction::Up: cur_anim = &(is_attacking ? anim_up_attack : anim_up_idle ); break;
		case PlayerDragonMgr::Direction::Down: cur_anim = &(is_attacking ?anim_down_attack : anim_down_idle ); break;
		case PlayerDragonMgr::Direction::Left: cur_anim = &(is_attacking ? anim_left_attack : anim_left_idle ); break;
		case PlayerDragonMgr::Direction::Right: cur_anim = &(is_attacking ? anim_right_attack  : anim_right_idle); break;
		}
		cur_anim->set_position(position);
		cur_anim->on_update(delta);
		if (is_attacking) cur_anim_vfx->on_update(delta);
	}

	void on_render(SDL_Renderer* renderer)
	{
		cur_anim->on_render(renderer);
		if (is_attacking) cur_anim_vfx->on_render(renderer);
	}

	float get_mp_percentage() const
	{
		return (float)mp / 100.0f;
	}

private:
	void attack()
	{
		can_normal_attack = false;
		is_attacking = true;

		// 设置hit_box/特效动画
		attack_hit_box->set_valid(true);			// 群攻
		switch (direction)
		{
		case PlayerDragonMgr::Direction::Up:
			attack_hit_box->set_size({ 68, 300 });
			attack_hit_box->set_position(position - Vector2(0, 96 / 2 + 300 / 2));
			anim_vfx_up_attack.set_position(position - Vector2(0, 96 / 2 + 300 / 2));
			cur_anim_vfx = &anim_vfx_up_attack;
			break;
		case PlayerDragonMgr::Direction::Down:
			attack_hit_box->set_size({ 68, 300 });
			attack_hit_box->set_position(position + Vector2(0, 96 / 2 + 300 / 2));
			anim_vfx_down_attack.set_position(position + Vector2(0, 96 / 2 + 300 / 2));
			cur_anim_vfx = &anim_vfx_down_attack;
			break;
		case PlayerDragonMgr::Direction::Left:
			attack_hit_box->set_size({ 300, 68 });
			attack_hit_box->set_position(position - Vector2(96 / 2 + 300 / 2, 0));
			anim_vfx_left_attack.set_position(position - Vector2(96 / 2 + 300 / 2, 0));
			cur_anim_vfx = &anim_vfx_left_attack;
			break;
		case PlayerDragonMgr::Direction::Right:
			attack_hit_box->set_size({ 300, 68 });
			attack_hit_box->set_position(position + Vector2(96 / 2 + 300 / 2, 0));
			anim_vfx_right_attack.set_position(position + Vector2(96 / 2 + 300 / 2, 0));
			cur_anim_vfx = &anim_vfx_right_attack;
			break;
		}
		cur_anim_vfx->restart();
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_Flash), 0);
	}

	void skill()
	{
		mp = 0;
		can_normal_attack = false;
		is_attacking = true;

		// 设置hit_box/特效动画
		skill_hit_box->set_valid(true);			// 群攻
		switch (direction)
		{
		case PlayerDragonMgr::Direction::Up:
			skill_hit_box->set_size({ 140, 60 });
			skill_hit_box->set_position(position - Vector2(0, 96 / 2 + 60 / 2));
			anim_vfx_up_skill.set_position(position - Vector2(0, 96 / 2 + 60 / 2));
			cur_anim_vfx = &anim_vfx_up_skill;
			break;
		case PlayerDragonMgr::Direction::Down:
			skill_hit_box->set_size({ 140, 60 });
			skill_hit_box->set_position(position + Vector2(0, 96 / 2 + 60 / 2));
			anim_vfx_down_skill.set_position(position + Vector2(0, 96 / 2 + 60 / 2));
			cur_anim_vfx = &anim_vfx_down_skill;
			break;
		case PlayerDragonMgr::Direction::Left:
			skill_hit_box->set_size({ 60, 140 });
			skill_hit_box->set_position(position - Vector2(96 / 2 + 60 / 2, 0));
			anim_vfx_left_skill.set_position(position - Vector2(96 / 2 + 60 / 2, 0));
			cur_anim_vfx = &anim_vfx_left_skill;
			break;
		case PlayerDragonMgr::Direction::Right:
			skill_hit_box->set_size({ 60, 140 });
			skill_hit_box->set_position(position + Vector2(96 / 2 + 60 / 2, 0));
			anim_vfx_right_skill.set_position(position + Vector2(96 / 2 + 60 / 2, 0));
			cur_anim_vfx = &anim_vfx_right_skill;
			break;
		}
		cur_anim_vfx->restart();
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_Impact), 0);
	}


private:
	PlayerTemplate attribute;					// 基础属性
	Timer timer_attack;							// 普攻定时器
	Timer timer_skill;							// 蓝条恢复定时器
	bool can_normal_attack = true;				// 能否普通攻击
	bool is_attacking = false;					// 是否正在攻击
	int mp = 100;								// 技能蓝条
	CollisionBox* attack_hit_box = nullptr;		// 闪电普通攻击碰撞盒子
	CollisionBox* skill_hit_box = nullptr;		// 技能冲击波攻击碰撞盒子
	CollisionBox* pick_box = nullptr;			// 拾取金币碰撞盒子

	// 控制相关:移动/攻击
	Direction direction = Direction::Left;
	Vector2 velocity;
	Vector2 position;
	bool is_up_keydown = false;
	bool is_down_keydown = false;
	bool is_left_keydown = false;
	bool is_right_keydown = false;
	bool is_attack_keydown = false;
	bool is_skill_keydown = false;

	// 动画相关
	Animation anim_up_idle;
	Animation anim_down_idle;
	Animation anim_left_idle;
	Animation anim_right_idle;
	Animation anim_up_attack;
	Animation anim_down_attack;
	Animation anim_left_attack;
	Animation anim_right_attack;
	Animation* cur_anim = nullptr;
	// 特效动画
	Animation anim_vfx_up_attack;
	Animation anim_vfx_down_attack;
	Animation anim_vfx_left_attack;
	Animation anim_vfx_right_attack;
	Animation anim_vfx_up_skill;
	Animation anim_vfx_down_skill;
	Animation anim_vfx_left_skill;
	Animation anim_vfx_right_skill;
	Animation* cur_anim_vfx = nullptr;
};

