#pragma once

#include "enemy_template.h"
#include "vector2.h"
#include "animation.h"
#include "config_manager.h"
#include "collision_manager.h"
#include "observer.h"

#include "SDL.h"
#include <cassert>

class Enemy : public Subject
{
public:
	Enemy()
	{
		// 动画初始化
		anim_up.set_interval(0.1);
		anim_down.set_interval(0.1);
		anim_left.set_interval(0.1);
		anim_right.set_interval(0.1);
		anim_up_sketch.set_interval(0.1);
		anim_down_sketch.set_interval(0.1);
		anim_left_sketch.set_interval(0.1);
		anim_right_sketch.set_interval(0.1);

		anim_up.set_loop(true);
		anim_down.set_loop(true);
		anim_left.set_loop(true);
		anim_right.set_loop(true);
		anim_up_sketch.set_loop(true);
		anim_down_sketch.set_loop(true);
		anim_left_sketch.set_loop(true);
		anim_right_sketch.set_loop(true);

		cur_anim = &anim_left;

		// 定时器初始化:受击闪烁效果/减速debuff
		timer_flash_duration.set_one_shot(true);
		timer_flash_duration.set_wait_time(0.6);
		timer_flash_duration.set_on_timeout([this]()
			{
				is_flashing_duration = false;
				is_flashing = false;
			});
		timer_flash_exchange.set_one_shot(false);
		timer_flash_exchange.set_wait_time(0.3);
		timer_flash_exchange.set_on_timeout([this]()
			{
				if(is_flashing_duration)
					is_flashing = !is_flashing;
			});
		timer_slow.set_one_shot(true);
		timer_slow.set_wait_time(2.0);
		timer_slow.set_on_timeout([this]()
			{
				is_slow = false;
			});

		// 碰撞盒初始化
		hurt_box = CollisionMgr::instance()->create_collision_box();
		hurt_box->set_size({ TILE_SIZE * 0.55, TILE_SIZE * 0.65 });
		hurt_box->set_src_layer(CollisionLayer::Enemy);
		hurt_box->set_dst_layer(CollisionLayer::Home);
		hurt_box->set_user_data(this);
	}
	~Enemy()
	{
		CollisionMgr::instance()->destroy_collision_box(hurt_box);
		// 通知观察者(塔防索敌优先级判断保留指针可能失效)，对象已经失效，取消关注
		Subject::notify_observer();		
	}

public:
	virtual void on_update(float delta)
	{
		if (!is_valid) return;

		// 更新位置
		Vector2 next_pos = get_route_next_postion();
		velocity = (next_pos - position).normalize() * attribute.speed * TILE_SIZE * (is_slow ? slow_scale : 1);
		position += velocity * delta;
		// 碰撞盒子位置
		hurt_box->set_position(position);

		// 动画更新:位置,帧
		if (std::abs(velocity.x) > std::abs(velocity.y))
		{
			if (velocity.x > 0) cur_anim = &(is_flashing ? anim_right_sketch : anim_right);
			else cur_anim = &(is_flashing ? anim_left_sketch : anim_left);
		}
		else
		{
			if (velocity.y > 0) cur_anim = &(is_flashing ? anim_down_sketch : anim_down);
			else cur_anim = &(is_flashing ? anim_up_sketch : anim_up);
		}
		cur_anim->set_position(position);
		cur_anim->on_update(delta);

		// 受击闪烁
		timer_flash_duration.on_update(delta);
		timer_flash_exchange.on_update(delta);

		// 减速效果
		if (is_slow) timer_slow.on_update(delta);
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		if (!is_valid) return;
		cur_anim->on_render(renderer);

		// 绘制血条
		render_hp_bar(renderer);
	}

	bool can_remove() const
	{
		return !is_valid;
	}

	void set_position(const Vector2& pos)
	{
		this->position = pos;
	}

	void make_invalid()
	{
		is_valid = false;
	}

	// 减速debuff
	void slow(float duration_time = 4.0, float slow_scale = 0.5)
	{
		is_slow = true;
		this->slow_scale = slow_scale;
		timer_slow.set_wait_time(duration_time);
		timer_slow.restart();
	}

	int get_hp() const
	{
		return cur_hp;
	}

	void decrease_hp(int val)
	{
		cur_hp = std::max(0, cur_hp - val);
		if (cur_hp <= 0) make_invalid();

		timer_flash_duration.restart();
		is_flashing_duration = true;
	}

	void increase_hp(int val)
	{
		cur_hp = std::min(attribute.hp, cur_hp + val);
	}

	float get_route_finish_ratio() const
	{
		// debug:这里忘记判断数组越界，当Enemy失效销毁后，持有Enemy指针的代码调用,数据未知，导致越界
		// (逻辑已经修改，但越界检测保留)
		assert(spawn_point >= 1 && spawn_point < ConfigMgr::instance()->route_list.size());
		if (spawn_point <= 0 || spawn_point >= ConfigMgr::instance()->route_list.size()) return 0;
		return (float)route_idx / (float)(ConfigMgr::instance()->route_list[spawn_point].size() - 1);
	}

	void set_spawn_point(int val)
	{
		spawn_point = val;

		ConfigMgr* config = ConfigMgr::instance();
		spawn_point = std::max(1, spawn_point);
		if (spawn_point >= config->route_list.size()) spawn_point = 1;
		auto& route = config->route_list[spawn_point];
		auto& tile_map = config->tile_map;

		// 判断下一个格子，将格子坐标转化为世界坐标
		int grid_x = route[route_idx].x, grid_y = route[route_idx].y;
		int world_x = grid_x * TILE_SIZE + TILE_SIZE / 2;
		int world_y = grid_y * TILE_SIZE + TILE_SIZE / 2;
		position.x = world_x;
		position.y = world_y;
		direction = tile_map[grid_y][grid_x].direction;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	int get_damage() const
	{
		return attribute.damage;
	}

	float get_reward_ration() const
	{
		return attribute.reward_ratio;
	}

	float get_recover_range() const
	{
		return attribute.recover_range * TILE_SIZE;
	}

	float get_recover_intensity() const
	{
		return attribute.recover_intensity;
	}

protected:
	void render_hp_bar(SDL_Renderer* renderer)
	{
		SDL_Rect rect_bg = { (int)(position.x - 30 / 2), (int)(position.y - TILE_SIZE / 2 - 6 - 2), 30, 6 };
		SDL_Rect rect_fg = { rect_bg.x + 2, rect_bg.y + 2, (int)(rect_bg.w * ((float)cur_hp / attribute.hp)) - 4, rect_bg.h - 4};
		SDL_SetRenderDrawColor(renderer, 186, 222, 209, 255);
		SDL_RenderFillRect(renderer, &rect_bg);
		SDL_SetRenderDrawColor(renderer, 58, 220, 161, 255);
		SDL_RenderFillRect(renderer, &rect_fg);
	}


private:
	// 获取下一个行进点坐标
	Vector2 get_route_next_postion()
	{
		assert(spawn_point >= 1 && spawn_point < ConfigMgr::instance()->route_list.size());
		if (spawn_point >= ConfigMgr::instance()->route_list.size()) spawn_point = 1;
		spawn_point = std::max(1, spawn_point);
		auto& route = ConfigMgr::instance()->route_list[spawn_point];
		auto& tile_map = ConfigMgr::instance()->tile_map;

		// 判断下一个格子，将格子坐标转化为世界坐标
		int grid_x = route[route_idx].x, grid_y = route[route_idx].y;
		float world_x = grid_x * TILE_SIZE + TILE_SIZE / 2;
		float world_y = grid_y * TILE_SIZE + TILE_SIZE / 2;
		Vector2 grid_center = { world_x, world_y };
		Vector2 next_pos = grid_center;
		// 如果递达当前目标格子，前往下一个格子
		if ((position - grid_center).lenth() < TILE_SIZE / 6 && route_idx + 1 < route.size())
		{
			route_idx++;
			int next_grid_x = route[route_idx].x, next_grid_y = route[route_idx].y;
			direction = tile_map[next_grid_y][next_grid_x].direction;
			next_pos.x = next_grid_x * TILE_SIZE + TILE_SIZE / 2;
			next_pos.y = next_grid_y * TILE_SIZE + TILE_SIZE / 2;
		}
		return next_pos;
	}


protected:
	EnemyTemplate attribute;			// 敌人基本属性
	int cur_hp = 100;					// 当前生命值
	bool is_valid = true;				// 是否有效

	// 移动
	Vector2 position;					// 位置
	Vector2 velocity;					// 速度
	Timer timer_slow;					// 减速定时器
	bool is_slow = false;				// 是否移动减速
	float slow_scale = 0.5;				// 减速比例
	int spawn_point = 0;				// 敌人出生点
	int route_idx = 0;					// 敌人行进路线当前下标
	Tile::Direction direction			// 移动方向
		= Tile::Direction::None;

	// 碰撞
	CollisionBox* hurt_box = nullptr;	// 受击碰撞层

	// 动画上下左右/受击闪烁
	Animation anim_up;
	Animation anim_down;
	Animation anim_left;
	Animation anim_right;
	Animation anim_up_sketch;
	Animation anim_down_sketch;
	Animation anim_left_sketch;
	Animation anim_right_sketch;
	Animation* cur_anim = nullptr;		// 当前动画
	bool is_flashing = false;			// 当前是否闪烁
	bool is_flashing_duration = false;	// 闪烁是否结束
	Timer timer_flash_duration;			// 闪烁持续时间定时器
	Timer timer_flash_exchange;			// 闪烁定时器
};
