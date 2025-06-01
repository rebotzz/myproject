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
	float speed = 4.1f;								// �ƶ��ٶȣ���λ����/��
	Animation anim_explode;							// ��ը����
	bool is_exploding = false;						// �Ƿ�ը
	float explode_range = 2.0f;						// ��ը��Χ����λ����
	Vector2 explode_position;						// ��ը���ĵ�����
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
			// ���е����˺�
			Enemy* enemy = reinterpret_cast<Enemy*>(hit_box->get_on_collide_box()->get_user_data());
			if (!enemy) return;
			enemy->decrease_hp(damage);
			// ��ը��ɷ�Χ�˺�
			// debug:��ը��Χ�˺�ʹ�ñ�ը��ײ��������bug���ظ���ͬһĿ������˺������Ƿ�Χ�˺�������Ϊֱ�ӱ���
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
