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
		// ������ʼ��
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

		// ��ʱ����ʼ��:�ܻ���˸Ч��/����debuff
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

		// ��ײ�г�ʼ��
		hurt_box = CollisionMgr::instance()->create_collision_box();
		hurt_box->set_size({ TILE_SIZE * 0.55, TILE_SIZE * 0.65 });
		hurt_box->set_src_layer(CollisionLayer::Enemy);
		hurt_box->set_dst_layer(CollisionLayer::Home);
		hurt_box->set_user_data(this);
	}
	~Enemy()
	{
		CollisionMgr::instance()->destroy_collision_box(hurt_box);
		// ֪ͨ�۲���(�����������ȼ��жϱ���ָ�����ʧЧ)�������Ѿ�ʧЧ��ȡ����ע
		Subject::notify_observer();		
	}

public:
	virtual void on_update(float delta)
	{
		if (!is_valid) return;

		// ����λ��
		Vector2 next_pos = get_route_next_postion();
		velocity = (next_pos - position).normalize() * attribute.speed * TILE_SIZE * (is_slow ? slow_scale : 1);
		position += velocity * delta;
		// ��ײ����λ��
		hurt_box->set_position(position);

		// ��������:λ��,֡
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

		// �ܻ���˸
		timer_flash_duration.on_update(delta);
		timer_flash_exchange.on_update(delta);

		// ����Ч��
		if (is_slow) timer_slow.on_update(delta);
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		if (!is_valid) return;
		cur_anim->on_render(renderer);

		// ����Ѫ��
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

	// ����debuff
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
		// debug:���������ж�����Խ�磬��EnemyʧЧ���ٺ󣬳���Enemyָ��Ĵ������,����δ֪������Խ��
		// (�߼��Ѿ��޸ģ���Խ���Ᵽ��)
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

		// �ж���һ�����ӣ�����������ת��Ϊ��������
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
	// ��ȡ��һ���н�������
	Vector2 get_route_next_postion()
	{
		assert(spawn_point >= 1 && spawn_point < ConfigMgr::instance()->route_list.size());
		if (spawn_point >= ConfigMgr::instance()->route_list.size()) spawn_point = 1;
		spawn_point = std::max(1, spawn_point);
		auto& route = ConfigMgr::instance()->route_list[spawn_point];
		auto& tile_map = ConfigMgr::instance()->tile_map;

		// �ж���һ�����ӣ�����������ת��Ϊ��������
		int grid_x = route[route_idx].x, grid_y = route[route_idx].y;
		float world_x = grid_x * TILE_SIZE + TILE_SIZE / 2;
		float world_y = grid_y * TILE_SIZE + TILE_SIZE / 2;
		Vector2 grid_center = { world_x, world_y };
		Vector2 next_pos = grid_center;
		// ����ݴﵱǰĿ����ӣ�ǰ����һ������
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
	EnemyTemplate attribute;			// ���˻�������
	int cur_hp = 100;					// ��ǰ����ֵ
	bool is_valid = true;				// �Ƿ���Ч

	// �ƶ�
	Vector2 position;					// λ��
	Vector2 velocity;					// �ٶ�
	Timer timer_slow;					// ���ٶ�ʱ��
	bool is_slow = false;				// �Ƿ��ƶ�����
	float slow_scale = 0.5;				// ���ٱ���
	int spawn_point = 0;				// ���˳�����
	int route_idx = 0;					// �����н�·�ߵ�ǰ�±�
	Tile::Direction direction			// �ƶ�����
		= Tile::Direction::None;

	// ��ײ
	CollisionBox* hurt_box = nullptr;	// �ܻ���ײ��

	// ������������/�ܻ���˸
	Animation anim_up;
	Animation anim_down;
	Animation anim_left;
	Animation anim_right;
	Animation anim_up_sketch;
	Animation anim_down_sketch;
	Animation anim_left_sketch;
	Animation anim_right_sketch;
	Animation* cur_anim = nullptr;		// ��ǰ����
	bool is_flashing = false;			// ��ǰ�Ƿ���˸
	bool is_flashing_duration = false;	// ��˸�Ƿ����
	Timer timer_flash_duration;			// ��˸����ʱ�䶨ʱ��
	Timer timer_flash_exchange;			// ��˸��ʱ��
};
