#pragma once
#include "player.h"
#include "sun_bullet.h"
#include "sun_ex_bullet.h"

// ���տ�
extern Atlas atlas_player_sunflower_idle_left;
extern Atlas atlas_player_sunflower_idle_right;
extern Atlas atlas_player_sunflower_run_left;
extern Atlas atlas_player_sunflower_run_right;
extern Atlas atlas_player_sunflower_attack_ex_right;
extern Atlas atlas_player_sunflower_attack_ex_left;

extern Atlas atlas_sunflower_sun_text;
extern Atlas atlas_player_sunflower_die_left;
extern Atlas atlas_player_sunflower_die_right;

extern std::shared_ptr<Player> player_1;		// ���1
extern std::shared_ptr<Player> player_2;		// ���2

class PlayerSunflower : public Player
{
private:
	Animation _animation_sun_text;						// ������⹥��ʱ�ı�����

	bool _sun_text_visible = false;						// "�Ƿ���ʾͷ���ı�����
	const Vector2 _velocity_sun = { 0.25f, -0.6f };		// �չ��ӵ��ٶ�
	const float _speed_sun_ex= 0.15f;					// �����չ��ӵ������ٶ�

public:
	PlayerSunflower(bool face_right = true) :Player(face_right)
	{
		// ��ʼ��
		_animation_idle_left.set_atlas(&atlas_player_sunflower_idle_left);
		_animation_idle_right.set_atlas(&atlas_player_sunflower_idle_right);
		_animation_run_left.set_atlas(&atlas_player_sunflower_run_left);
		_animation_run_right.set_atlas(&atlas_player_sunflower_run_right);
		_animation_die_left.set_atlas(&atlas_player_sunflower_die_left);
		_animation_die_right.set_atlas(&atlas_player_sunflower_die_right);

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

		_animation_attack_ex_left.set_atlas(&atlas_player_sunflower_attack_ex_left);
		_animation_attack_ex_right.set_atlas(&atlas_player_sunflower_attack_ex_right);
		_animation_sun_text.set_atlas(&atlas_sunflower_sun_text);
		static const int attack_ex_frame_interval_ms = 100;
		_animation_attack_ex_left.set_interval(attack_ex_frame_interval_ms);
		_animation_attack_ex_right.set_interval(attack_ex_frame_interval_ms);
		_animation_sun_text.set_interval(attack_ex_frame_interval_ms);
		_animation_attack_ex_left.set_loop(false);
		_animation_attack_ex_right.set_loop(false);
		_animation_sun_text.set_loop(false);

		// ����״̬ʱ��Ϊ���ж���ʱ��
		_animation_attack_ex_left.set_callback([&]() {
			_is_attack_ex = false;
			});
		_animation_attack_ex_right.set_callback([&]() {
			_is_attack_ex = false;
			});
		_animation_sun_text.set_callback([&]() {
			_sun_text_visible = false;
			});

		_attack_cd = 400;
		_timer_attack_cd.set_wait_time(_attack_cd);
		_timer_attack_ex.set_callback([]() {});		// ʹ�ô��ж����������״̬,ȡ����ʱ���ص�

		// �����ײ����
		_size.x = 96;
		_size.y = 96;
	}

	~PlayerSunflower() = default;

	virtual void on_update(int interval_ms) override
	{
		Player::on_update(interval_ms);

		if (_sun_text_visible)
			_animation_sun_text.on_update(interval_ms);
	}

	virtual void on_draw(const Camera& camera) const  override
	{
		Player::on_draw(camera);

		if (_sun_text_visible)
		{
			// ��"�ı�����λ��
			Vector2 position_sun_text;							
			const IMAGE* frame_sun_text = _animation_sun_text.get_frame();
			position_sun_text.x = _position.x + (_size.x - frame_sun_text->getwidth()) / 2;
			position_sun_text.y = _position.y - frame_sun_text->getheight();
			_animation_sun_text.on_draw(camera, (int)position_sun_text.x, (int)position_sun_text.y);
		}
	}

	virtual void on_attack() override
	{
		Bullet* sun_bullet = new SunBullet;
		// �ӵ�����λ�������տ�ͷ��
		Vector2 bullet_position;
		const Vector2& bullet_size = sun_bullet->get_size();
		bullet_position.x = _position.x + (_size.x - bullet_size.x) / 2;
		bullet_position.y = _position.y;

		sun_bullet->set_damage(15);
		sun_bullet->set_callback([&]() { _mp += 35; });
		sun_bullet->set_collide_target(_id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		sun_bullet->set_position(bullet_position);
		sun_bullet->set_velocity(_is_face_right ? _velocity_sun.x	: -_velocity_sun.x, _velocity_sun.y);

		bullet_list.push_back(sun_bullet);
	}

	virtual void on_attack_ex() override
	{
		_is_attack_ex = true;
		_sun_text_visible = true;
		_animation_sun_text.reset();
		_is_face_right ? _animation_attack_ex_right.reset() : _animation_attack_ex_left.reset();

		Bullet* sun_ex_bullet = new SunExBullet;
		// �ӵ�����λ���ڶ���ͷ��������
		Vector2 bullet_position;
		const Vector2& bullet_size = sun_ex_bullet->get_size();
		const Player* target = (_id == PlayerID::P1 ? player_2.get() : player_1.get());
		const Vector2& target_pos = target->get_position();
		const Vector2& target_size = target->get_size();
		bullet_position.x = target_pos.x + (target_size.x - bullet_size.x) / 2;
		bullet_position.y = -bullet_size.y;

		sun_ex_bullet->set_damage(15);
		sun_ex_bullet->set_callback([&]() { _mp += 50; });
		sun_ex_bullet->set_collide_target(_id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		sun_ex_bullet->set_position(bullet_position);
		sun_ex_bullet->set_velocity(0, _speed_sun_ex);

		bullet_list.push_back(sun_ex_bullet);

		mciSendString(L"play sun_text from 0", nullptr, 0, nullptr);
	}

};