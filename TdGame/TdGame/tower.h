#pragma once

#include "tower_template.h"
#include "timer.h"
#include "animation.h"
#include "collision_manager.h"
#include "home_manager.h"
#include "bullet_manager.h"
#include "observer.h"
#include "enemy.h"

#include "SDL.h"


// 防御塔
class Tower : public Observer
{
private:
	enum class Direction
	{
		Up, Down, Left, Right
	};

public:
	Tower(TowerType tower_type_, const Vector2& position_) :tower_type(tower_type_), position(position_)
	{
		// 基本属性
		switch (tower_type)
		{
		case TowerType::Archer: attribute = ConfigMgr::instance()->tower_archer_template; break;
		case TowerType::Axeman: attribute = ConfigMgr::instance()->tower_axeman_template; break;
		case TowerType::Gunner: attribute = ConfigMgr::instance()->tower_gunner_template; break;
		}
		level = HomeMgr::instance()->get_tower_level(tower_type);

		// 攻击定时器
		timer_attack.set_one_shot(true);
		timer_attack.set_wait_time(get_attack_interval());
		timer_attack.set_on_timeout([this]()
			{
				can_shot = true;
			});
		timer_aim.set_one_shot(false);
		timer_aim.set_wait_time(0.1);
		timer_aim.set_on_timeout([this]()
			{
				can_aim_enemy = true;
				target_enemy = nullptr;
			});


		// 视野范围：索敌
		view_range_box = CollisionMgr::instance()->create_collision_box();
		view_range_box->set_dst_layer(CollisionLayer::Enemy);
		view_range_box->set_position(position);
		view_range_box->set_shape_mode(CollisionBox::ShapeMode::Circle);
		view_range_box->set_radius(get_view_range());
		view_range_box->set_on_collide([this]()
			{
				if (!can_aim_enemy) return;
				can_aim_enemy = false;

				// 寻找视野范围内离home最近的敌人作为目标
				if (!view_range_box->get_on_collide_box() || !view_range_box->get_on_collide_box()->get_user_data()) return;
				Enemy* enemy = reinterpret_cast<Enemy*>(view_range_box->get_on_collide_box()->get_user_data());
				// debug: 防止target_enemy失效，内存释放后，数据未知，调用报错问题。
				// 解决：观察者模式，注册目标敌人的观察者，当失效时，取消target
				if (!enemy || enemy->can_remove()) return;
				if (!target_enemy || target_enemy->can_remove()
					|| enemy->get_route_finish_ratio() > target_enemy->get_route_finish_ratio())
				{
					// 索敌更新：避免瞄准目标失效
					if (target_enemy) target_enemy->detach_observer(this);
					target_enemy = enemy;
					target_enemy->attach_observer(this, [this]() { target_enemy = nullptr; });
				}
				// 开启攻击动画
				if (!is_attacking)
				{
					is_attacking = true;
					switch (direcition)
					{
					case Tower::Direction::Up: anim_attack_up.restart(); break;
					case Tower::Direction::Down: anim_attack_down.restart(); break;
					case Tower::Direction::Left: anim_attack_left.restart(); break;
					case Tower::Direction::Right: anim_attack_right.restart(); break;
					}
				}
			});

		// 等级更新:注册观察者
		HomeMgr::instance()->attach_observer(this);

		// 动画:具体子类完成
		anim_idle_up.set_loop(true);
		anim_idle_down.set_loop(true);
		anim_idle_left.set_loop(true);
		anim_idle_right.set_loop(true);
		anim_attack_up.set_loop(true);	
		anim_attack_down.set_loop(true);
		anim_attack_left.set_loop(true);
		anim_attack_right.set_loop(true);

		anim_idle_up.set_position(position);
		anim_idle_down.set_position(position);
		anim_idle_left.set_position(position);
		anim_idle_right.set_position(position);
		anim_attack_up.set_position(position);
		anim_attack_down.set_position(position);
		anim_attack_left.set_position(position);
		anim_attack_right.set_position(position);
		cur_anim = &anim_idle_down;
	}

	virtual ~Tower()
	{
		CollisionMgr::instance()->destroy_collision_box(view_range_box);
		HomeMgr::instance()->detach_observer(this);
	}

public:
	virtual void on_update(float delta)
	{
		// 攻击/索敌更新
		timer_attack.on_update(delta);
		timer_aim.on_update(delta);
		if (can_shot && target_enemy && !target_enemy->can_remove()) shot_bullet();
		else is_attacking = false;

		// 动画更新
		cur_anim->on_update(delta);
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		cur_anim->on_render(renderer);
	}

	void Observer::update() override
	{
		// HomeMgr:防御塔升级，更新视野范围/攻击间隔/伤害
		level = HomeMgr::instance()->get_tower_level(tower_type);
		view_range_box->set_radius(get_view_range());
		timer_attack.set_wait_time(get_attack_interval());
	}

private:
	void shot_bullet()
	{
		can_shot = false;
		timer_attack.restart();

		int damage = 10;
		BulletType bullet_type = BulletType::Arrow;
		switch (tower_type)
		{
		case TowerType::Archer:
			damage = ConfigMgr::instance()->tower_archer_template.damage[level];
			bullet_type = BulletType::Arrow;
			break;
		case TowerType::Axeman:
			damage = ConfigMgr::instance()->tower_axeman_template.damage[level];
			bullet_type = BulletType::Axe;
			break;
		case TowerType::Gunner:
			damage = ConfigMgr::instance()->tower_gunner_template.damage[level];
			bullet_type = BulletType::Shell;
			break;
		}

		Vector2 shot_direction = (target_enemy->get_position() - position);
		target_enemy = nullptr;		// 射击完成，重新锁定目标
		BulletMgr::instance()->create_bullet(bullet_type, shot_direction, position, damage);

		// 更新射击动画方向
		if (std::abs(shot_direction.x) > std::abs(shot_direction.y))
		{
			if (shot_direction.x > 0) direcition = Direction::Right;
			else direcition = Direction::Left;
		}
		else
		{
			if (shot_direction.y > 0) direcition = Direction::Down;
			else direcition = Direction::Up;
		}
		switch (direcition)
		{
		case Tower::Direction::Up:
			cur_anim = &(is_attacking ? anim_attack_up : anim_idle_up);
			break;
		case Tower::Direction::Down:
			cur_anim = &(is_attacking ? anim_attack_down : anim_idle_down);
			break;
		case Tower::Direction::Left:
			cur_anim = &(is_attacking ? anim_attack_left : anim_idle_left);
			break;
		case Tower::Direction::Right:
			cur_anim = &(is_attacking ? anim_attack_right : anim_idle_right);
			break;
		}
	}

	float get_view_range() const
	{
		return attribute.view_range[level] * TILE_SIZE;
	}

	int get_damage() const
	{
		return attribute.damage[level];
	}

	float get_attack_interval() const
	{
		return attribute.interval[level];
	}


protected:
	// 基本属性
	TowerTemplate attribute;					// 属性
	int level = 0;								// 当前防御塔等级
	TowerType tower_type = TowerType::Archer;	// 防御塔类型
	Vector2 position;							// 位置

	// 攻击
	Timer timer_attack;							// 攻击计时器
	bool can_shot = false;						// 能否射击
	bool is_attacking = false;					// 是否在攻击(动画判断)
	Enemy* target_enemy = nullptr;				// 索敌目标,注意目标失效处理(观察者模式)
	CollisionBox* view_range_box = nullptr;		// 视野范围碰撞盒子
	//// 可有可无
	Timer timer_aim;							// 索敌定时器(避免频繁索敌)
	bool can_aim_enemy = true;					// 能否索敌

	// 动画:闲置/攻击
	Animation anim_idle_up;
	Animation anim_idle_down;
	Animation anim_idle_left;
	Animation anim_idle_right;
	Animation anim_attack_up;
	Animation anim_attack_down;
	Animation anim_attack_left;
	Animation anim_attack_right;
	Direction direcition = Direction::Left;
	Animation* cur_anim = nullptr;
};
