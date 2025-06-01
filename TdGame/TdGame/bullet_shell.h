#pragma once

#include "bullet.h"
#include "resources_manager.h"
#include "enemy_manager.h"


class BulletShell : public Bullet
{
public:
	BulletShell(const Vector2& direction, const Vector2& init_position);
	~BulletShell() = default;

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;

protected:
	void play_hit_sound() override;

private:
	float speed = 4.1f;								// 移动速度，单位格子/秒
	Animation anim_explode;							// 爆炸动画
	bool is_exploding = false;						// 是否爆炸
	float explode_range = 2.0f;						// 爆炸范围，单位格子
	Vector2 explode_position;						// 爆炸中心的坐标
};

BulletShell::BulletShell(const Vector2& direction, const Vector2& init_position)
{
	set_velocity(direction.normalize() * speed * TILE_SIZE);
	set_position(init_position);

	anim.set_frame(ResMgr::instance()->find_texture(ResID::Tex_BulletShell), TILE_SIZE, TILE_SIZE, { 0,1 });
	anim.set_loop(true);
	anim.set_interval(0.2);
	anim_explode.set_frame(ResMgr::instance()->find_texture(ResID::Tex_EffectExplode), TILE_SIZE * 2, TILE_SIZE * 2, { 0,1,2,3,4 });
	anim_explode.set_loop(false);
	anim_explode.set_interval(0.1);
	anim_explode.set_on_finished([this]()
		{
			is_valid = false;
		});

	hit_box->set_on_collide([this]()
		{
			hit_box->set_valid(false);
			is_exploding = true;
			// 命中敌人伤害
			Enemy* enemy = reinterpret_cast<Enemy*>(hit_box->get_on_collide_box()->get_user_data());
			if (!enemy) return;
			enemy->decrease_hp(damage);
			// 爆炸造成范围伤害
			// debug:爆炸范围伤害使用爆炸碰撞盒子遇到bug（重复对同一目标造成伤害，不是范围伤害），改为直接遍历
			explode_position = position + velocity.normalize() * TILE_SIZE / 2;
			anim_explode.set_position(explode_position);
			auto& enemy_list = EnemyMgr::instance()->get_enemy_list();
			for (Enemy* enemy_ptr : enemy_list)
			{
				if (enemy_ptr->can_remove()) continue;
				if ((enemy_ptr->get_position() - explode_position).lenth() < explode_range * TILE_SIZE)
					enemy_ptr->decrease_hp(damage * 0.5);
			}
		});

	Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_ShellFire), 0);
}

void BulletShell::on_update(float delta)
{
	Bullet::on_update(delta);
	if (is_exploding)
		anim_explode.on_update(delta);
}

void BulletShell::on_render(SDL_Renderer* renderer)
{
	(is_exploding ? anim_explode : anim).on_render(renderer);
}

void BulletShell::play_hit_sound()
{
	Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_ShellHit), 0);
}
