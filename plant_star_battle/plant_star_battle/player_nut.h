#pragma once
#include "player.h"
#include "nut_explode_bullet.h"

extern Camera main_camera;

// ��� 
extern Atlas atlas_player_nut_idle_left;
extern Atlas atlas_player_nut_idle_right;
extern Atlas atlas_player_nut_run_left;
extern Atlas atlas_player_nut_run_right;
extern Atlas atlas_player_nut_attack_ex_left;
extern Atlas atlas_player_nut_attack_ex_right;
extern Atlas atlas_player_nut_die_left;
extern Atlas atlas_player_nut_die_right;


class PlayerNut : public Player
{
private:
	Timer _timer_recovery;				// �Զ���Ѫ������ʱ��
	int _last_hp = 10;					// ��һ֡��Ѫ��

public:
	PlayerNut(bool face_right = true) :Player(face_right)
	{
		// ������Դ��ʼ��
		_animation_idle_left.set_atlas(&atlas_player_nut_idle_left);
		_animation_idle_right.set_atlas(&atlas_player_nut_idle_right);
		_animation_run_left.set_atlas(&atlas_player_nut_run_left);
		_animation_run_right.set_atlas(&atlas_player_nut_run_right);
		_animation_attack_ex_left.set_atlas(&atlas_player_nut_attack_ex_left);
		_animation_attack_ex_right.set_atlas(&atlas_player_nut_attack_ex_right);
		_animation_die_left.set_atlas(&atlas_player_nut_die_left);
		_animation_die_right.set_atlas(&atlas_player_nut_die_right);

		_animation_idle_left.set_interval(75);
		_animation_idle_right.set_interval(75);
		_animation_run_left.set_interval(75);
		_animation_run_right.set_interval(75);
		_animation_die_left.set_interval(150);
		_animation_die_right.set_interval(150);

		_animation_idle_left.set_loop(true);
		_animation_idle_right.set_loop(true);
		_animation_run_left.set_loop(true);
		_animation_run_right.set_loop(true);
		_animation_die_left.set_loop(false);
		_animation_die_right.set_loop(false);

		_animation_attack_ex_left.set_loop(false);
		_animation_attack_ex_right.set_loop(false);
		_animation_attack_ex_left.set_callback([&]()
			{
				_hp -= 20;
				nut_explode();
				_is_attack_ex = false;
			}
		);
		_animation_attack_ex_right.set_callback([&]()
			{
				_hp -= 20;
				nut_explode();
				_is_attack_ex = false;
			}
		);

		_animation_attack_ex_left.set_interval(75);
		_animation_attack_ex_right.set_interval(75);

		_attack_cd = 400;
		_timer_attack_cd.set_wait_time(_attack_cd);
		//_timer_attack_ex.set_wait_time(75 * atlas_player_sunflower_attack_ex_left.get_size());
		_timer_attack_ex.set_callback([]() {});	// ����ʹ�ö�ʱ�������,����ȡ��,ʹ�ö��������ص�

		// �����ײ����
		_size.x = 96;
		_size.y = 96;

		// �Զ���Ѫ������ʱ����ʼ��
		_timer_recovery.set_wait_time(500);
		_timer_recovery.set_one_shot(false);
		_timer_recovery.set_callback([&]()
			{
				if (_hp <= 0)
					return;

				_hp += 1;
				_mp += 1;
				if (_hp > 100) _hp = 100;
				if (_mp > 100) _mp = 100;
			}
		);

		_mp = 50;
	}

	~PlayerNut()
	{}

	virtual void on_update(int interval_ms) override
	{
		_last_hp = _hp;

		// ��ײ���,������,�������µ��߼�
		Player::on_update(interval_ms);

		// ������˻�������
		if (_hp < _last_hp)
			_mp += (_last_hp - _hp) * 2;

		// ����Զ���Ѫ��������
		_timer_recovery.on_update(interval_ms);
	}

	virtual void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);
	}

private:
	virtual void on_attack() override
	{
		// ��ͨ����Ч��:˲��
		const int distance = 150;
		_is_face_right ? _position.x += distance : _position.x -= distance;

		mciSendString(L"play nut_dash from 0", nullptr, 0, nullptr);
	}

	virtual void on_attack_ex() override
	{
		_is_attack_ex = true;
		_is_face_right ? _animation_attack_ex_right.reset() : _animation_attack_ex_left.reset();
	}

	void nut_explode()
	{
		NutExBullet* nut_ex_bullet = new NutExBullet;

		Vector2 bullet_position;
		const Vector2& bullet_size = nut_ex_bullet->get_size();
		// ��ը�ӵ�����λ���ڼ������
		bullet_position.x = _position.x + (_size.x - bullet_size.x) / 2;
		bullet_position.y = _position.y + (_size.y - bullet_size.y) / 2;
		nut_ex_bullet->set_collide_target(_id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		nut_ex_bullet->set_position(bullet_position);
		nut_ex_bullet->set_velocity({ 0, 0 });
		nut_ex_bullet->set_damage(50);
		nut_ex_bullet->set_callback([&]()
			{
				_mp += 50;
			}
		);

		bullet_list.push_back(nut_ex_bullet);

		mciSendString(L"play nut_explode from 0", nullptr, 0, nullptr);
	}

};
