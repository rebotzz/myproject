#include "player_state_node.h"
#include "player.h"
#include "character_manager.h"
#include "audio_manager.h"
#include "scene_manager.h"
#include "particle_manager.h"
#include "effect.h"

void PlayerAttackState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("attack");

	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->set_attacking(true);
	player->on_attack();
	player->get_hit_box()->set_enabled(true);
	player->update_hit_box_position();

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
	// 更新过程中可能会状态跳转
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->update_hit_box_position();

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
	else if (player->can_dance())
		player->switch_state("dance");

}


void PlayerDanceState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("dance");

	// todo:或许可以播放特殊的BGM
}
void PlayerDanceState::on_update(float delta)
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->set_velocity({ 0 , 0 });

	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (!player->can_dance())
	{
		if (player->can_attack())
			player->switch_state("attack");
		else if (player->can_roll())
			player->switch_state("roll");
		else if (player->get_move_axis() != 0)
			player->switch_state("run");
		else if (player->can_jump())
			player->switch_state("jump");
		else if (player->get_move_axis() == 0)
			player->switch_state("idle");
	}

}
void PlayerDanceState::on_exit()
{
	// 关闭播放特殊的BGM
}


PlayerDeadState::PlayerDeadState()
{
}
void PlayerDeadState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("dead");
	AudioManager::instance()->play_audio_ex(_T("player_dead"));

	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->set_velocity({ 0, 0 });
	player->on_jump(0.5f);
	player->enable_displace_ex(player->get_facing_redir(), player->get_stay_air_time());
	player->get_hurt_box()->set_enabled(false);
}

void PlayerDeadState::on_update(float delta)
{

}
void PlayerDeadState::on_exit()
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->get_hurt_box()->set_enabled(true);
}




