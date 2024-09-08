#include "player_state_node.h"
#include "character_manager.h"
#include "player.h"
#include "audio_manager.h"

void PlayerAttackState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("attack");

	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->set_attacking(true);
	player->on_attack(); 
	player->get_hit_box()->set_enabled(true);
	update_hit_box_position();

	switch (random_range(1, 3))
	{
	case 1:
		 AudioManager::instance()->play_audio_ex(_T("player_attack_1"));
		break;
	case 2:
		 AudioManager::instance()->play_audio_ex(_T("player_attack_2"));
		break;
	case 3:
		 AudioManager::instance()->play_audio_ex(_T("player_attack_3"));
		break;
	}
}

void PlayerAttackState::on_update(float delta)
{
	update_hit_box_position();

	// 更新过程中可能会状态跳转
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (!player->get_attacking())
	{
		if (player->get_velocity().y > 0)
			player->switch_state("fall");
		else if (player->get_move_axis() == 0)
			player->switch_state("idle");
		else if (player->is_on_floor() && player->get_move_axis() != 0)
			player->switch_state("run");
	}
}

void PlayerAttackState::on_exit()
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->get_hit_box()->set_enabled(false);
}

void PlayerAttackState::update_hit_box_position()
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	const Vector2& pos_center = player->get_logic_center();
	CollisionBox* hit_box = player->get_hit_box();
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;

	switch (player->get_attack_dir())
	{
	case Player::AttackDir::Up:
		pos_hit_box = { pos_center.x, pos_center.y - size_hit_box.y / 2 };
		break;
	case Player::AttackDir::Down:
		pos_hit_box = { pos_center.x, pos_center.y + size_hit_box.y / 2 };
		break;
	case Player::AttackDir::Left:
		pos_hit_box = { pos_center.x - size_hit_box.x / 2, pos_center.y };
		break;
	case Player::AttackDir::Right:
		pos_hit_box = { pos_center.x + size_hit_box.x / 2, pos_center.y };
		break;
	}
	hit_box->set_position(pos_hit_box);
}


void PlayerRunState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("run");

	 AudioManager::instance()->play_audio_ex(_T("player_run"), true);
}

void PlayerRunState::on_update(float delta)
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());

	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->can_attack())
		player->switch_state("attack");
	else if (player->can_roll())
		player->switch_state("roll");
	else if (player->can_jump())
		player->switch_state("jump");
	else if (player->get_move_axis() == 0)
		player->switch_state("idle");
}

void PlayerRunState::on_exit()
{
	AudioManager::instance()->stop_audio_ex(_T("player_run"));
}


void PlayerRollState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("roll");

	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->set_rolling(true);
	player->get_hurt_box()->set_enabled(false);
	player->on_roll();

	AudioManager::instance()->play_audio_ex(_T("player_roll"));
}

void PlayerRollState::on_update(float delta)
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());

	if (!player->get_rolling())
	{
		if (player->get_move_axis() != 0)
			player->switch_state("run");
		else if (player->can_jump())
			player->switch_state("jump");
		else 
			player->switch_state("idle");
	}
}

void PlayerRollState::on_exit()
{
	CharacterManager::instance()->get_player()->get_hurt_box()->set_enabled(true);
}


void PlayerJumpState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("jump");

	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->on_jump();

	AudioManager::instance()->play_audio_ex(_T("player_jump"));
}

void PlayerJumpState::on_update(float delta)
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());

	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->can_attack())
		player->switch_state("attack");
	else if (player->get_velocity().y > 0)
		player->switch_state("fall");
}


void PlayerFallState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("fall");
}
void PlayerFallState::on_update(float delta)
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());

	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->can_attack())
		player->switch_state("attack");
	else if (player->is_on_floor())
	{
		player->on_land();
		player->switch_state("idle");

		AudioManager::instance()->play_audio_ex(_T("player_land"));
	}

}


void PlayerIdleState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("idle");
}
void PlayerIdleState::on_update(float delta)
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());

	// 跳转状态
	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->can_attack())
		player->switch_state("attack");
	else if (player->get_velocity().y > 0)
		player->switch_state("fall");
	else if (player->can_jump())
		player->switch_state("jump");
	else if (player->can_roll())
		player->switch_state("roll");
	else if (player->is_on_floor() && player->get_move_axis() != 0)
		player->switch_state("run");

}


PlayerDeadState::PlayerDeadState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(2.0f);
	timer.set_on_timeout([]()
		{
			MessageBox(GetHWnd(), _T("不对......\n这样不行"), _T("角色死亡"), MB_OK);
			exit(0);
		});
}

void PlayerDeadState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("dead");
	timer.restart();
	AudioManager::instance()->play_audio_ex(_T("player_dead"));
}
void PlayerDeadState::on_update(float delta)
{
	timer.on_update(delta);
}
void PlayerDeadState::on_exit()
{
	// todo: 时间逆流
}