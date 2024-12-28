#pragma once
#include "vector2.h"
#include "animation.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "resources_manager.h"
#include "bullet.h"
#include <ctime>
#include <memory>
#include <vector>

extern Camera scene_camera;

// ����
class Turrent
{
protected:
	Vector2 pos_self;									// ����λ��
	Vector2 pos_target;									// ���λ��
	Animation anim_barrel_fire;							// ǹ���������
	Animation anim_barrel_idle;							// ǹ�����ö�����ͼƬ��
	Animation anim_bottom;								// �����ײ�ʯͷ������ͼƬ��
	Animation anim_battery;								// �����ײ���ض�����ͼƬ��
	Animation anim_crosshair;							// ��׼ʮ�ֶ�����ͼƬ��
	bool is_fire_keydown = false;						// �������״̬
	bool is_cool_down = true;							// ��ǹ�Ƿ���ȴ
	std::vector<std::shared_ptr<Bullet>> bullet_list;	// �ӵ��б�

public:
	Turrent(const Vector2& positon)
		:pos_self(positon)
	{
		anim_barrel_fire.set_loop(false);
		anim_barrel_fire.set_interval(0.04f);
		anim_barrel_fire.add_frame(ResourcesManager::instance()->find_atlas("barrel_fire"));
		anim_barrel_fire.set_position(pos_self + Vector2({100, 0}));
		anim_barrel_fire.set_rotate_center(25, 25);
		anim_barrel_fire.set_on_finished([&]()
			{
				is_cool_down = true;
			});

		anim_barrel_idle.set_loop(false);
		anim_barrel_idle.set_interval(0.1f);
		anim_barrel_idle.add_frame(ResourcesManager::instance()->find_image("barrel_idle"));
		anim_barrel_idle.set_position(pos_self + Vector2({ 100, 0 }));
		anim_barrel_idle.set_rotate_center(25, 25);

		anim_bottom.set_loop(false);
		anim_bottom.set_interval(0.1f);
		anim_bottom.add_frame(ResourcesManager::instance()->find_image("bottom"));
		anim_bottom.set_position(pos_self);

		anim_battery.set_loop(false);
		anim_battery.set_interval(0.1f);
		anim_battery.add_frame(ResourcesManager::instance()->find_image("battery"));
		anim_battery.set_position(pos_self);
		anim_battery.set_rotate_center(62.5, 62.5);

		anim_crosshair.set_loop(false);
		anim_crosshair.set_interval(0.1f);
		anim_crosshair.add_frame(ResourcesManager::instance()->find_image("crosshair"));
		anim_crosshair.set_position(pos_self);
	}

	virtual void on_input(SDL_Event* event)
	{
		switch (event->type)
		{
		case SDL_MOUSEMOTION:
			pos_target.x = (float)event->button.x;
			pos_target.y = (float)event->button.y;
			break;
		case SDL_MOUSEBUTTONDOWN:								
			is_fire_keydown = true;
			break;
		case SDL_MOUSEBUTTONUP:
			is_fire_keydown = false;
			break;
		}
	}

	virtual void on_update(float delta)
	{
		// �����ӵ�,�л�/���¶���
		if (is_fire_keydown && is_cool_down)
		{
			anim_barrel_fire.reset();
			is_cool_down = false;
			spawn_bullet();
		}

		if (!is_cool_down)
		{
			anim_barrel_fire.on_update(delta);
			//scene_camera.shake(3.0f, 0.3f);

			//SDL_Log("not cool down... [%d]\n", is_cool_down);
		}
		//scene_camera.on_update(delta);



		double shot_angle = (pos_target - pos_self).angle();
		anim_barrel_fire.set_angle(shot_angle);
		anim_barrel_idle.set_angle(shot_angle);
		anim_battery.set_angle(shot_angle);
		anim_crosshair.set_position(pos_target);

		// �����ӵ�
		for (auto& bullet : bullet_list)
		{
			bullet->on_update(delta);
		}

		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(), [](std::shared_ptr<Bullet> bullet)
			{
				return !bullet->is_valid();
			}), bullet_list.end());
	}

	virtual void on_render(const Camera& camera) const
	{
		// �ӵ�
		for (auto& bullet : bullet_list)
		{
			bullet->on_render(camera);
		}

		// ����,ǹ�ܣ���׼
		anim_battery.on_render(camera);
		if (is_cool_down)
		{
			anim_barrel_idle.on_render(camera);
		}
		else
		{
			anim_barrel_fire.on_render(camera);
		}
		anim_crosshair.on_render(camera);
	}

	void spawn_bullet()
	{
		// �����ӵ�	
		Vector2 pos_barrel_front = pos_self + (pos_target - pos_self).normalize() * 100.0;
		Vector2 shot_dir = pos_target - pos_self;
		Vector2 vertical_offset = Vector2(shot_dir.y, shot_dir.x).normalize() * (rand() % 2 ? 1 : -1) * (rand() % 6);
		std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(pos_barrel_front + vertical_offset, shot_dir);
		bullet_list.push_back(bullet);

		switch (rand() % 3 + 1)
		{
		case 1:
			Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio_chunk("fire_1"), 0);
			break;
		case 2:
			Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio_chunk("fire_2"), 0);
			break;
		case 3:
			Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio_chunk("fire_3"), 0);
			break;
		}
	}

	void set_position(const Vector2& target)
	{
		pos_self = target;

		anim_barrel_fire.set_position(target);
		anim_barrel_idle.set_position(target);
		anim_bottom.set_position(target);
		anim_battery.set_position(target);
		anim_crosshair.set_position(target);
	}

	void render_bottom(const Camera& camera) const
	{
		anim_bottom.on_render(camera);
	}
};

// �Զ�����
class TurrentAuto : public Turrent
{
private:
	enum class Mode
	{
		Strafing,	// ����ɨ��
		AutoAim		// �Զ���׼ todo
	};

private:
	double angle = 0.0;				// ��ǰ�Ƕ�
	double center_angle = 0.0;		// ɨ�����ĽǶ�
	double delta_angle = 45.0;		// ɨ��ڶ��Ƕ�+/-
	Timer timer_strafe;				// �ڶ��Ƕȶ�ʱ��
	double speed_angle = 15.0;		// �ڶ����ٶ�
	bool dir_clockwise = true;		// �ڶ�����

private:
	const double SCALE = 0.5;		// �������ű���(����)

public:
	TurrentAuto(const Vector2& positon, double center_angle_ = -90.0, 
		double delta_angle_ = 45.0, bool dir_clockwise_ = true)
		:Turrent(positon)
	{
		angle = center_angle = center_angle_;
		delta_angle = delta_angle_;
		dir_clockwise = dir_clockwise_;

		timer_strafe.set_one_shot(false);
		timer_strafe.set_wait_time(0.01);
		timer_strafe.set_on_timeout([&]()
			{
				if (abs(angle - center_angle) >= delta_angle)
				{
					dir_clockwise = !dir_clockwise;
				}

				angle += dir_clockwise ? 0.01 * speed_angle : -0.01 * speed_angle;

				double radian = angle * 3.1415926 / 180.0;
				pos_target.x = pos_self.x + 50 * cos(radian);
				pos_target.y = pos_self.y + 50 * sin(radian);
			});
	}

	virtual void on_input(SDL_Event* event) override
	{
		// ��ʱʲôҲ����������Ԥ����������Ƕ� | �л�ģʽ
	}

	virtual void on_update(float delta) override
	{
		timer_strafe.on_update(delta);

		anim_barrel_fire.set_angle(angle);
		anim_battery.set_angle(angle);
		anim_barrel_fire.on_update(delta);

		// �����ӵ�
		for (auto& bullet : bullet_list)
		{
			bullet->on_update(delta);
		}

		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(), [](std::shared_ptr<Bullet> bullet)
			{
				return !bullet->is_valid();
			}), bullet_list.end());
	}

	virtual void on_render(const Camera& camera) const
	{
		// �ӵ�
		for (auto& bullet : bullet_list)
		{
			bullet->on_render(camera);
		}

		// ����,ǹ�ܣ���׼
		anim_battery.on_render(camera);
		anim_barrel_fire.on_render(camera);
	}

	void set_center_angle(double val)
	{
		angle = center_angle = val;
	}
};