#include "enemy_hornet_state_node.h"
#include "enemy_hornet.h"
#include "character_manager.h"
#include "audio_manager.h"
#include "scene_manager.h"
#include "particle_manager.h"
#include "effect.h"


// ��Ʒ�AI�߼�1.0	�������Բ����������ı�,����û�к�ҡ,��ҡȫ��idle״̬
// ��Ʒ�AI�߼�2.0	�����������������ı�,���Ӳ��ֹ�����ҡ

EnemyHornetIdleState::EnemyHornetIdleState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(1.5);
	timer.set_on_timeout([&]
		{
			// ���������,������ת״̬
			Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
			if (player->get_hp() <= 0)
				return;

			// ����: 1.��Ѫ����[���ؽ�ս����] 2.��Ѫ����[����Զ�̹���] 
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			float distance = abs(hornet->get_position().x - player->get_position().x);

			if (hornet->get_hp() == hornet->get_hp_max() / 2 && is_first_half_hp)
			{
				is_first_half_hp = false;
				hornet->switch_state("throw_barbs");
				return;
			}

			int rand_num = random_range(0, 100);
			if (hornet->get_hp() >= hornet->get_hp_max() / 2)
			{
				if (rand_num <= 25)
					hornet->switch_state("jump");			// 25% ��Ծ
				else if (rand_num <= 50)
					hornet->switch_state("run");			// 25% ����
				else if (rand_num <= 80)
					hornet->switch_state("squat");			// 30% �¶�(׼�����)
				else if(rand_num <= 90 && distance <= MIN_DIS)
					hornet->switch_state("throw_silk");		// 10% ˿�߹���
				else
					hornet->switch_state("throw_sword");	// 10% �ӳ���
			}
			else
			{
				if (rand_num <= 25)
					hornet->switch_state("jump");			// 25% ��Ծ
				else if (rand_num <= 45)
					hornet->switch_state("throw_barbs");	// 20% �ӳ�����
				else if(rand_num <= 55)
					hornet->switch_state("squat");			// 10% �¶�(׼�����)
				else if (rand_num <= 65 && distance <= MIN_DIS * 1.5f)
					hornet->switch_state("throw_silk");		// 10% ˿�߹���
				else 
					hornet->switch_state("throw_sword");	// 35% �ӳ���
			}
		});
}

void EnemyHornetIdleState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("idle");

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->set_velocity({ 0, 0 });

	float wait_time = 0.15f;
	if (hornet->get_hp() >= hornet->get_hp_max() / 2)
		wait_time += (float)random_range(0, 50) * 0.01f;		// 0.15s + 0.0s ~ 0.5s
	else
		wait_time += (float)random_range(0, 25) * 0.01f;		// 0.15s + 0.0s ~ 0.25s

	timer.set_wait_time(wait_time);
	timer.restart();
}
void EnemyHornetIdleState::on_update(float delta)
{
	timer.on_update(delta);

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	hornet->set_facing_left(pos_player.x < pos_hornet.x);

	// [����]����ת״̬: ���� ��Ծ ���� �¶� �ӽ� �Ӵ��� ��˿��
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if (hornet->get_velocity().y > 0)
		hornet->switch_state("fall");
}
void EnemyHornetIdleState::on_exit()
{
	// ���½�ɫ����
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
	hornet->set_facing_left(player->get_position().x < hornet->get_position().x);
}


void EnemyHornetJumpState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("jump");

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
	const Vector2& pos_hornet = hornet->get_position();
	const Vector2& pos_player = player->get_position();
	float distance = abs(pos_hornet.x - pos_player.x);

	// ��������Ҿ��������Ծ����,���½�ɫ����,"����","ǰ��"
	float x_speed = 0.0f;
	int rand_num = random_range(0, 100);
	// Զ�����
	if (distance <= MIN_DIS)
	{
		x_speed = (pos_hornet.x < pos_player.x ? -SPEED_MOVE_AXIS : SPEED_MOVE_AXIS);
		hornet->set_facing_left(x_speed > 0);

		if (hornet->get_hp() >= hornet->get_hp_max() / 2)
		{
			if (rand_num <= 60)
				next_state = NextState::Aim;			// 60% 
			else if (rand_num <= 65)
				next_state = NextState::ThrowSilk;		// 5%
			else
				next_state = NextState::Fall;			// 45%
		}
		else
		{
			if (rand_num <= 70)
				next_state = NextState::Aim;			// 70% 
			else if (rand_num <= 75)
				next_state = NextState::ThrowSilk;		// 5%
			else
				next_state = NextState::Fall;			// 35%
		}
	}
	// �ӽ����
	else
	{
		x_speed = (pos_hornet.x < pos_player.x ? SPEED_MOVE_AXIS : -SPEED_MOVE_AXIS);
		hornet->set_facing_left(x_speed < 0);

		if (hornet->get_hp() >= hornet->get_hp_max() / 2)
		{
			if (rand_num <= 70)
				next_state = NextState::Aim;			// 70% 
			else if (rand_num <= 80)
				next_state = NextState::ThrowSilk;		// 10%
			else
				next_state = NextState::Fall;			// 10%
		}
		else
		{
			if (rand_num <= 80)
				next_state = NextState::Aim;			// 80% 
			else if (rand_num <= 90)
				next_state = NextState::ThrowSilk;		// 10%
			else
				next_state = NextState::Fall;			// 10%
		}
	}

	hornet->set_velocity({ x_speed, -SPEED_JUMP });
}

void EnemyHornetJumpState::on_update(float delta)
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [��Ծ]����ת״̬: ���� ���� ��׼ ��˿��  ->���ƶ��ӽ����
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if (hornet->get_velocity().y > 0)
	{
		switch (next_state)
		{
		case NextState::Fall:
			hornet->switch_state("fall");
			break;
		case NextState::Aim:
			hornet->switch_state("aim");
			break;
		case NextState::ThrowSilk:
			hornet->switch_state("throw_silk");
			break;
		}
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
	hornet->set_facing_left(pos_player.x < pos_hornet.x);
	hornet->set_velocity({ pos_player.x < pos_hornet.x ? -SPEED_RUN : SPEED_RUN, 0 });


	// [����]����ת״̬: ���� �¶� ��˿��  ��Ծ?
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if (abs(pos_player.x - pos_hornet.x) <= MIN_DISTANCE)
	{
		int num = random_range(0, 100);
		if (hornet->get_hp() >= hornet->get_hp_max() / 2)
		{
			if (num <= 75)
				hornet->switch_state("squat");			// 75%
			else
				hornet->switch_state("throw_silk");		// 25%
		}
		else
		{
			if (num <= 75)
				hornet->switch_state("throw_silk");
			else
				hornet->switch_state("squat");			// 25%
		}
	}
}
void EnemyHornetRunState::on_exit()
{
	CharacterManager::instance()->get_enemy()->set_velocity({ 0, 0 });

	AudioManager::instance()->stop_audio_ex(_T("enemy_run"));
}


void EnemyHornetFallState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("fall");
}

void EnemyHornetFallState::on_update(float delta)
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [����]����ת״̬: ���� ����
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if (hornet->is_on_floor())
		hornet->switch_state("idle");
}



EnemyHornetDashInAirState::EnemyHornetDashInAirState()
{
	// todo: �Ƿ��Ҫ,�����Ծ�߶ȳ������,�ǾͲ��������
	// ���⼫�����,��ʱ�ص�����״̬
	timer.set_one_shot(true);
	timer.set_wait_time(3.0f);
	timer.set_on_timeout([]()
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->switch_state("idle");
		});
}

void EnemyHornetDashInAirState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("dash_in_air");

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
	hornet->set_velocity((player->get_position() - hornet->get_position()).normalize() * SPEED_DASH);
	hornet->set_facing_left(player->get_position().x < hornet->get_position().x);
	hornet->set_dashing_in_air(true);
	hornet->set_gravity_enable(false);
	hornet->on_dash();
	timer.restart();

	AudioManager::instance()->play_audio_ex(_T("enemy_dash"));
	AudioManager::instance()->play_audio_ex(_T("hornet_say_dash"));
}

void EnemyHornetDashInAirState::on_update(float delta)
{
	timer.on_update(delta);
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [���г��]����ת״̬: ���� ����
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
	else if (hornet->is_on_floor())
		hornet->switch_state("idle");
}

void EnemyHornetDashInAirState::on_exit()
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->set_dashing_in_air(false);
	hornet->set_gravity_enable(true);
}



EnemyHornetDashOnFloorState::EnemyHornetDashOnFloorState()
{
	timer_dash.set_one_shot(true);
	timer_dash.set_wait_time(0.35f);
	timer_dash.set_on_timeout([]()
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->set_dashing_on_floor(false);
			hornet->set_velocity({ 0, 0 });
			hornet->set_facing_left(CharacterManager::instance()->get_player()->get_position().x < hornet->get_position().x);

			if (hornet->get_control() == nullptr)
			{
				if (random_range(0, 100) <= 75)
					hornet->switch_state("idle");			// 75%
				else
					hornet->switch_state("throw_sword");	// 25%
			}
			else
			{
				hornet->switch_state("idle");
			}
		});
}

void EnemyHornetDashOnFloorState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("dash_on_floor");

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->set_velocity({ hornet->get_facing_left() ? -SPEED_DASH : SPEED_DASH, 0 });
	hornet->set_dashing_on_floor(true);
	hornet->on_dash();
	timer_dash.restart();

	AudioManager::instance()->play_audio_ex(_T("enemy_dash"));
	AudioManager::instance()->play_audio_ex(_T("hornet_say_dash"));
}

void EnemyHornetDashOnFloorState::on_update(float delta)
{
	timer_dash.on_update(delta);
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

	// [������]����ת״̬: ���� �ӽ� ����
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
}

void EnemyHornetDashOnFloorState::on_exit()
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
}



EnemyHornetAimState::EnemyHornetAimState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout([]()
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->set_gravity_enable(true);
			hornet->switch_state("dash_in_air");
		});
}
void EnemyHornetAimState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("aim");

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->set_gravity_enable(false);
	hornet->set_velocity({ 0, 0 });
	timer.restart();
}
void EnemyHornetAimState::on_update(float delta)
{
	timer.on_update(delta);

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
}

EnemyHornetSquatState::EnemyHornetSquatState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout([]()
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->switch_state("dash_on_floor");
		});
}
void EnemyHornetSquatState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("squat");

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
	hornet->set_facing_left(player->get_position().x < hornet->get_position().x);
	timer.restart();
}
void EnemyHornetSquatState::on_update(float delta)
{
	timer.on_update(delta);

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
}


EnemyHornetThrowSwordState::EnemyHornetThrowSwordState()
{
	timer_throw.set_one_shot(true);
	timer_throw.set_wait_time(0.65f);
	timer_throw.set_on_timeout([&]()
		{
			// �ڶ�����;��,Ч������
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->throw_sword();

			AudioManager::instance()->play_audio_ex(_T("enemy_throw_sword"));
			AudioManager::instance()->play_audio_ex(_T("hornet_say_throw_sword"));
		});

	timer_switch.set_one_shot(true);
	timer_switch.set_wait_time(1.0f);
	timer_switch.set_on_timeout([&]()
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());

			if (hornet->get_control() == nullptr)
			{
				int rand_num = random_range(0, 100);
				if (hornet->get_hp() >= hornet->get_hp_max() / 2)
				{
					if (rand_num <= 50)
						hornet->switch_state("squat");		// 50%
					else if (rand_num <= 80)
						hornet->switch_state("jump");		// 30%
					else
						hornet->switch_state("idle");		// 20%
				}
				else
				{
					if (rand_num <= 50)
						hornet->switch_state("jump");		// 50%
					else if (rand_num <= 80)
						hornet->switch_state("throw_silk");	// 30%
					else
						hornet->switch_state("idle");		// 20%
				}
			}
			else
			{
				hornet->switch_state("idle");
			}
		});
}
void EnemyHornetThrowSwordState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("throw_sword");

	timer_throw.restart();
	timer_switch.restart();
}
void EnemyHornetThrowSwordState::on_update(float delta)
{
	timer_throw.on_update(delta);
	timer_switch.on_update(delta);

	// [�ӽ�]����ת״̬: ���� ���� �¶� ��Ծ
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
}


EnemyHornetThrowBarbsState::EnemyHornetThrowBarbsState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(0.8f);
	timer.set_on_timeout([]
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->throw_barbs();
			hornet->switch_state("idle");
		});
}
void EnemyHornetThrowBarbsState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("throw_barbs");

	timer.restart();

	AudioManager::instance()->play_audio_ex(_T("enemy_throw_barbs"));
	AudioManager::instance()->play_audio_ex(_T("hornet_final_yell"));
}
void EnemyHornetThrowBarbsState::on_update(float delta)
{
	timer.on_update(delta);

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	// [�Ӵ���]����ת״̬: ���� ����
	if (hornet->get_hp() <= 0)
		hornet->switch_state("dead");
}
void EnemyHornetThrowBarbsState::on_exit()
{
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
}




EnemyHornetThrowSilkState::EnemyHornetThrowSilkState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(0.9f);
	timer.set_on_timeout([]
		{
			EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
			hornet->set_throwing_silk(false);
			hornet->set_gravity_enable(true);

			if (hornet->get_control() == nullptr)
			{
				int rand_num = random_range(0, 100);
				if (!hornet->is_on_floor())
				{
					if (rand_num < 40)
						hornet->switch_state("aim");	// 40%
					else
						hornet->switch_state("fall");	// 60%
				}
				else
				{
					if (rand_num <= 80)
						hornet->switch_state("idle");	// 80%
					else
						hornet->switch_state("jump");	// 20%
				}
			}
			else
			{
				hornet->switch_state("idle");
			}
		});
}
void EnemyHornetThrowSilkState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("throw_silk");

	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	hornet->set_gravity_enable(false);
	hornet->set_throwing_silk(true);
	hornet->on_throw_silk();
	hornet->set_velocity({ 0, 0 });
	timer.restart();

	AudioManager::instance()->play_audio_ex(_T("hornet_say_throw_silk"));
	AudioManager::instance()->play_audio_ex(_T("enemy_throw_silk"));
}
void EnemyHornetThrowSilkState::on_update(float delta)
{
	timer.on_update(delta);

	// [��˿��]����ת״̬: ���� ���� ���� ��׼
	EnemyHornet* hornet = dynamic_cast<EnemyHornet*>(CharacterManager::instance()->get_enemy());
	if (hornet->get_hp() <= 0)
	{
		hornet->switch_state("dead");
	}
}

EnemyHornetDeadState::EnemyHornetDeadState()
{
	timer_dialogue.set_one_shot(true);
	timer_dialogue.set_wait_time(4.f);
	timer_dialogue.set_on_timeout([]
		{
			std::shared_ptr<EffectText> particle(new EffectText(
				_T("��������...���ǿն���ʿ"), 6.0f));
			particle->set_position({ (float)getwidth() / 2, (float)getheight() / 2 });
			ParticleManager::instance()->register_particle(particle);
			AudioManager::instance()->play_audio_ex(_T("hornet_dialogue"));
		});
}

void EnemyHornetDeadState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("idle");
	timer_dialogue.restart();

	std::shared_ptr<EffectText> text(new EffectText(
		_T("�ܺ�...��������"), 3.0f, RGB(0, 255, 255)));
	text->set_position({ (float)getwidth() / 2, (float)getheight() / 2 });
	ParticleManager::instance()->register_particle(text);

	AudioManager::instance()->play_audio_ex(_T("hornet_dead"));
}

void EnemyHornetDeadState::on_update(float delta)
{
	timer_dialogue.on_update(delta);
}