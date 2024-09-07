#include "player_state_node.h"
#include "character_manager.h"

PlayerAttackState::PlayerAttackState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(0.3f);
	timer.set_on_timeout([]()
		{
			Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
			player->set_attacking(false);
		});
}
PlayerAttackState::~PlayerAttackState() = default;

void PlayerAttackState::on_enter()
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->set_animation("attack");
	player->set_attacking(true);
	player->on_attack(); 
	player->get_hit_box()->set_enabled(true);
	update_hit_box_position();
	timer.restart();

	switch (random_range(1, 3))
	{
	case 1:
		play_audio(_T("player_attack_1"));
			break;
	case 2:
		play_audio(_T("player_attack_2"));
		break;
	case 3:
		play_audio(_T("player_attack_3"));
		break;
	}
}

void PlayerAttackState::on_update(float delta)
{
	timer.on_update(delta);
	update_hit_box_position();

	// 更新过程中可能会状态跳转
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (!player->can_attacking())
	{
		if (player->get_velocity().y > 0)
			player->switch_state("fall");
		else if (player->get_move_axis() == 0)
			player->switch_state("idle");
		else if (player->is_on_floor() && player->get_move_axis() != 0)
			player->switch_state("run");
		else if (player->is_on_floor() && player->can_jump())
			player->switch_state("jump");
	}

}

void PlayerAttackState::on_exit()
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	player->get_hit_box()->set_enabled(false);
	player->set_attacking(false);
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




PlayerIdleState::PlayerIdleState() = default;
PlayerIdleState::~PlayerIdleState() = default;
void PlayerIdleState::on_enter()
{

}
void PlayerIdleState::on_update(float delta)
{

}
void PlayerIdleState::on_exit()
{

}







PlayerDeadState::PlayerDeadState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(2.0f);
	timer.set_on_timeout([]()
		{
			MessageBox(GetHWnd(), _T("不对......\n这样不行"), _T("角色死亡"), MB_OK);
		});
}
PlayerDeadState::~PlayerDeadState() = default;
void PlayerDeadState::on_enter()
{

}
void PlayerDeadState::on_update(float delta)
{

}
void PlayerDeadState::on_exit()
{

}