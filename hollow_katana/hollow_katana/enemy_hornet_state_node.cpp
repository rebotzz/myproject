#include "enemy_hornet_state_node.h"
#include "enemy_hornet.h"
#include "character_manager.h"
#include "audio_manager.h"


EnemyHornetIdleState::EnemyHornetIdleState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(CD);
	timer.set_on_timeout([&]
		{
			can_switch_state = true;
		});
}

void EnemyHornetIdleState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("idle");

	timer.restart();
	float rand_time = (float)random_range(1, 30) / 100.0f;
	rand_time *= (random_range(1, 2) == 1 ? -1 : 1);
	timer.set_wait_time(CD + rand_time);
	can_switch_state = false;

	CharacterManager::instance()->get_enemy()->set_velocity({ 0, 0 });
}
void EnemyHornetIdleState::on_update(float delta)
{
	timer.on_update(delta);

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	float distance = (pos_hornet - pos_player).length();
	hornet->set_facing_left(pos_player.x < pos_hornet.x);

	// [闲置]可跳转状态: 死亡 跳跃 奔跑 瞄准 扔剑 扔刺球 扔丝线
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if(can_switch_state)
	{
		// 通过与玩家距离判断是否要接近玩家
		if (distance > (float)getwidth() / 2)
		{
			// 这里利用随机数 + 概率区间确定下一个状态
			int num = random_range(1, 100);
			if (num <= 20)
				hornet->switch_state("run");
			else if (num <= 50)
				hornet->switch_state("jump");
			else if (num <= 65)
				hornet->switch_state("throw_sword");
			else if (num <= 85)
				hornet->switch_state("aim");
			else if (num <= 95)
				hornet->switch_state("throw_barbs");
			else
				hornet->switch_state("throw_silk");
		}
		else
		{
			int num = random_range(1, 100);
			if (num <= 50)
				hornet->switch_state("aim");
			else if (num <= 70)
				hornet->switch_state("throw_sword");
			else if(num <= 85)
				hornet->switch_state("jump");
			else
			{
				if (random_range(1,2) == 1)
					hornet->switch_state("throw_barbs");
				else
					hornet->switch_state("throw_silk");
			}
		}
	}
}



void EnemyHornetJumpState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("jump");

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();

	is_jump_left = pos_player.x < pos_hornet.x;
	hornet->set_facing_left(is_jump_left);
	hornet->on_move_left(is_jump_left);
	hornet->on_jump();
}

void EnemyHornetJumpState::on_update(float delta)
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [跳跃]可跳转状态: 死亡 下落 空中冲刺 扔丝线  ->可移动接近玩家
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if (hornet->get_velocity().y > 0)
	{
		int num = random_range(1, 10);
		if (num <= 7)
			hornet->switch_state("dash_in_air");
		else if (num <= 8)
			hornet->switch_state("throw_silk");
		else
			hornet->switch_state("fall");
	}
}



void EnemyHornetRunState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("run");
	AudioManager::instance()->play_audio_ex(_T("enemy_run"), true);
}
void EnemyHornetRunState::on_update(float delta)
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	hornet->on_move_left(pos_player.x < pos_hornet.x);
	hornet->set_facing_left(pos_player.x < pos_hornet.x);
	float distance = (pos_player - pos_hornet).length();


	// [奔跑]可跳转状态: 死亡 闲置 跳跃 瞄准 扔剑 扔丝线	->可移动接近玩家
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if (distance <= (float)getwidth() / 2)
	{
		int num = random_range(1, 100);
		if (num <= 15)
			hornet->switch_state("jump");
		else if (num <= 60)
			hornet->switch_state("aim");
		else if (num <= 75)
			hornet->switch_state("throw_sword");
		else if (num <= 90)
		{
			if(distance <= 260)
				hornet->switch_state("throw_silk");
			else 
				hornet->switch_state("idle");
		}
	}
}
void EnemyHornetRunState::on_exit()
{
	AudioManager::instance()->stop_audio_ex(_T("enemy_run"));

	CharacterManager::instance()->get_enemy()->set_velocity({ 0, 0 });
}


void EnemyHornetFallState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("fall");
}

void EnemyHornetFallState::on_update(float delta)
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [下落]可跳转状态: 死亡 闲置 	->可移动接近玩家
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if(hornet->get_velocity().y == 0)
		hornet->switch_state("idle");
}



EnemyHornetDashAirState::EnemyHornetDashAirState()
{
	// 避免极端情况,超时回到闲置状态
	timer.set_one_shot(true);
	timer.set_wait_time(3.0f);
	timer.set_on_timeout([]()
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->switch_state("idle");
		});
}

void EnemyHornetDashAirState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("dash_in_air");

	timer.restart();

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	hornet->set_dash_speed((pos_player - pos_hornet).normalize());
	hornet->set_dashing_in_air(true);
	hornet->on_dash();
	hornet->set_gravity_enable(false);
	hornet->set_facing_left(pos_player.x < pos_hornet.x);

	AudioManager::instance()->play_audio_ex(_T("enemy_dash"));
	AudioManager::instance()->play_audio_ex(_T("hornet_say_dash"));
}

void EnemyHornetDashAirState::on_update(float delta)
{
	timer.on_update(delta);
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [空中冲刺]可跳转状态: 死亡 闲置
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if (hornet->is_on_floor())
		hornet->switch_state("idle");
}

void EnemyHornetDashAirState::on_exit()
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->set_dashing_in_air(false);
	hornet->set_gravity_enable(true);
}



EnemyHornetDashFloorState::EnemyHornetDashFloorState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(0.35f);
	timer.set_on_timeout([]()
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->switch_state("idle");
		});
}

void EnemyHornetDashFloorState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("dash_on_floor");

	timer.restart();

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	hornet->set_dash_speed({ (hornet->get_facing_left() ? -1.0f : 1.0f), 0});
	hornet->set_dashing_on_floor(true);
	hornet->on_dash();

	AudioManager::instance()->play_audio_ex(_T("enemy_dash"));
	AudioManager::instance()->play_audio_ex(_T("hornet_say_dash"));
}

void EnemyHornetDashFloorState::on_update(float delta)
{
	timer.on_update(delta);
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [地面冲刺]可跳转状态: 死亡 闲置
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
}

void EnemyHornetDashFloorState::on_exit()
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->set_dashing_on_floor(false);
}



EnemyHornetAimState::EnemyHornetAimState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(CD);
	timer.set_on_timeout([]()
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->switch_state("dash_on_floor");
		});
}

void EnemyHornetAimState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("aim");

	float rand_time = random_range(1, 10) / 100.0f;
	rand_time *= random_range(1, 2) == 1 ? -1 : 1;
	timer.set_wait_time(CD + rand_time);
	timer.restart();

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	hornet->set_facing_left(pos_player.x < pos_hornet.x);
}

void EnemyHornetAimState::on_update(float delta)
{
	timer.on_update(delta);
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [瞄准]可跳转状态: 死亡 地面冲刺
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
}



EnemyHornetThrowSwordState::EnemyHornetThrowSwordState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(0.8f);
	timer.set_on_timeout([&]()
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->switch_state("idle");
		});
}
void EnemyHornetThrowSwordState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("throw_sword");

	timer.restart();

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	hornet->set_facing_left(pos_player.x < pos_hornet.x);
}
void EnemyHornetThrowSwordState::on_update(float delta)
{
	timer.on_update(delta);

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [扔剑]可跳转状态: 死亡 闲置
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
}
void EnemyHornetThrowSwordState::on_exit()
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->throw_sword();

	AudioManager::instance()->play_audio_ex(_T("enemy_throw_sword"));
	AudioManager::instance()->play_audio_ex(_T("hornet_say_throw_sword"));
}



EnemyHornetThrowBarbsState::EnemyHornetThrowBarbsState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(0.8f);
	timer.set_on_timeout([]
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->switch_state("idle");
		});
}
void EnemyHornetThrowBarbsState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("throw_barbs");

	timer.restart();
}
void EnemyHornetThrowBarbsState::on_update(float delta)
{
	timer.on_update(delta);

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	hornet->set_facing_left(pos_player.x < pos_hornet.x);

	// [扔刺球]可跳转状态: 死亡 闲置
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
}
void EnemyHornetThrowBarbsState::on_exit()
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->throw_barbs();
	AudioManager::instance()->play_audio_ex(_T("enemy_throw_barbs"));
}




EnemyHornetThrowSilkState::EnemyHornetThrowSilkState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(1.35f);
	timer.set_on_timeout([]
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->set_throwing_silk(false);
		});
}
void EnemyHornetThrowSilkState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("throw_silk");

	timer.restart();

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->set_gravity_enable(false);
	hornet->set_throwing_silk(true);
	hornet->on_throw_silk();
	hornet->set_velocity({ 0, 0 });

	AudioManager::instance()->play_audio_ex(_T("hornet_say_throw_silk"));
	AudioManager::instance()->play_audio_ex(_T("enemy_throw_silk"), true);
}
void EnemyHornetThrowSilkState::on_update(float delta)
{
	timer.on_update(delta);

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [扔丝线]可跳转状态: 死亡 下落 闲置
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	if (!hornet->get_throwing_silk())
	{
		if(hornet->is_on_floor())
			hornet->switch_state("idle");
		else
			hornet->switch_state("fall");
	}
}
void EnemyHornetThrowSilkState::on_exit()
{
	CharacterManager::instance()->get_enemy()->set_gravity_enable(true);

	AudioManager::instance()->stop_audio_ex(_T("enemy_throw_silk"));
}



void EnemyHornetDeadState::on_enter()
{
	MessageBox(GetHWnd(), _T("很好...\n这样能行"), _T("挑战成功"), MB_OK);
	exit(0);
}