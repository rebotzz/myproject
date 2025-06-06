#include "character.h"

extern Camera* main_camera;

Character::Character()
{
	hit_box = CollisionManager::instance()->create_collision_box();
	hurt_box = CollisionManager::instance()->create_collision_box();
	interact_box = CollisionManager::instance()->create_collision_box();

	timer_invincible_status.set_one_shot(true);
	timer_invincible_status.set_wait_time(TIME_INVINCIBLE);
	timer_invincible_blink.set_one_shot(false);
	timer_invincible_blink.set_wait_time(0.075f);
	timer_invincible_status.set_on_timeout([&]()
		{
			is_invincible_status = false;
		});
	timer_invincible_blink.set_on_timeout([&]()
		{
			is_blink_invisible = !is_blink_invisible;
		});

	timer_platform_reset.set_one_shot(true);
	timer_platform_reset.set_wait_time(0.05f);
	timer_platform_reset.set_on_timeout([&]()
		{
			set_platform_floor_y();
		});
}

Character::~Character()
{
	CollisionManager::instance()->destroy_collision_box(hit_box);
	CollisionManager::instance()->destroy_collision_box(hurt_box);
	CollisionManager::instance()->destroy_collision_box(interact_box);
}

void Character::switch_state(const std::string& id)
{
	state_machine.switch_to(id);
}

void Character::make_invulnerable(bool not_blink_, float delta_ratio)
{
	not_blink = not_blink_;
	is_invincible_status = true;
	timer_invincible_status.set_wait_time(TIME_INVINCIBLE * delta_ratio);
	timer_invincible_status.restart();
	timer_invincible_blink.restart();
}

void Character::set_animation(const std::string& id)
{
	current_animation = &animation_pool[id];

	current_animation->left.reset();
	current_animation->right.reset();
}

void Character::decrease_hp()
{
	if (is_invincible_status || hp <= 0)
		return;

	hp -= 1;
	if (hp > 0)
		make_invulnerable();
	on_hurt();
}


void Character::on_update(float delta)
{
	// 状态机更新
	state_machine.on_update(delta);

	// 角色位置更新
	if (hp < 0)
		velocity.x = 0;
	if (enable_gravity)
		velocity.y += GRAVITY * delta;
	prev_frame_pos_y = position.y;		// 平台碰撞判定用
	position += velocity * delta;

	if (is_on_floor())
	{
		position.y = get_floor_y();
		velocity.y = 0;
	}
	if (position.x < 0)
		position.x = 0;
	else if (position.x > getwidth())
		position.x = (float)getwidth();

	hurt_box->set_position(get_logic_center());
	interact_box->set_position(get_logic_center());

	// 更新无敌状态计时器
	timer_invincible_status.on_update(delta);
	if (is_invincible_status && !not_blink)
		timer_invincible_blink.on_update(delta);

	// 更新角色动画
	if (!current_animation)
		return;
	Animation& animation = (is_facing_left ? current_animation->left : current_animation->right);
	animation.set_position(position);
	animation.on_update(delta);

	timer_platform_reset.on_update(delta);
}

void Character::on_render()
{
	if (!current_animation || (is_invincible_status && is_blink_invisible) || is_invisible)
		return;

	(is_facing_left ? current_animation->left : current_animation->right).on_render();
}
