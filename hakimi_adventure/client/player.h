#pragma once
#include "animation.h"

class Player
{
private:
	enum class Facing
	{
		Left, Right, Up, Down
	};

private:
	const float SPEED_RUN = 100.0f;			// ����ٶȱ���

private:
	// ��Ҷ���
	Animation anim_idle_left;		
	Animation anim_idle_right;
	Animation anim_idle_up;
	Animation anim_idle_down;
	Animation anim_run_left;
	Animation anim_run_right;
	Animation anim_run_up;
	Animation anim_run_down;
	Animation* anim_current = nullptr;		// ��ҵ�ǰ����	

	// ����ƶ�
	Vector2 position;						// ��ǰλ��
	Vector2 pos_target;						// Ŀ��λ��
	Vector2 velocity;						// ����ٶ�ʸ��
	// ��ҳ���
	Facing facing = Facing::Down;

public:
	Player(Atlas* atlas_idle_left, Atlas* atlas_idle_right, Atlas* atlas_idle_up, Atlas* atlas_idle_down,
		Atlas* atlas_run_left, Atlas* atlas_run_right, Atlas* atlas_run_up, Atlas* atlas_run_down)
	{
		anim_idle_left.set_loop(true);
		anim_idle_left.set_interval(0.1f);
		anim_idle_left.add_frame(atlas_idle_left);

		anim_idle_right.set_loop(true);
		anim_idle_right.set_interval(0.1f);
		anim_idle_right.add_frame(atlas_idle_right);

		anim_idle_up.set_loop(true);
		anim_idle_up.set_interval(0.1f);
		anim_idle_up.add_frame(atlas_idle_up);

		anim_idle_down.set_loop(true);
		anim_idle_down.set_interval(0.1f);
		anim_idle_down.add_frame(atlas_idle_down);

		anim_run_left.set_loop(true);
		anim_run_left.set_interval(0.1f);
		anim_run_left.add_frame(atlas_run_left);

		anim_run_right.set_loop(true);
		anim_run_right.set_interval(0.1f);
		anim_run_right.add_frame(atlas_run_right);

		anim_run_up.set_loop(true);
		anim_run_up.set_interval(0.1f);
		anim_run_up.add_frame(atlas_run_up);

		anim_run_down.set_loop(true);
		anim_run_down.set_interval(0.1f);
		anim_run_down.add_frame(atlas_run_down);
	}

	~Player() = default;

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	void set_target(const Vector2& pos_target)
	{
		this->pos_target = pos_target;
	}

	void on_update(float delta)
	{
		// ����λ��
		if (!position.approx(pos_target))
			velocity = (pos_target - position).normalize() * SPEED_RUN;
		else
			velocity = { 0, 0 };

		if ((pos_target - position).length() <= (velocity * delta).length())
			position = pos_target;
		else
			position += velocity * delta;

		// ���¶���
		if (!velocity.approx({ 0, 0 }))
		{
			if (abs(velocity.x) >= 0.0001f)
				facing = velocity.x > 0 ? Facing::Right : Facing::Left;
			if (abs(velocity.y) >= 0.0001f)
				facing = velocity.y > 0 ? Facing::Down : Facing::Up;

			switch (facing)
			{
			case Facing::Left: anim_current = &anim_run_left; break;
			case Facing::Right: anim_current = &anim_run_right; break;
			case Facing::Up: anim_current = &anim_run_up; break;
			case Facing::Down: anim_current = &anim_run_down; break;
			}
		}
		else
		{
			switch (facing)
			{
			case Facing::Left: anim_current = &anim_idle_left; break;
			case Facing::Right: anim_current = &anim_idle_right; break;
			case Facing::Up: anim_current = &anim_idle_up; break;
			case Facing::Down: anim_current = &anim_idle_down; break;
			}
		}

		if (!anim_current) return;
		anim_current->set_position(position);
		anim_current->on_update(delta);
	}

	void on_render(const Camera& camera)
	{
		if (!anim_current) return;

		anim_current->on_render(camera);
	}
};
