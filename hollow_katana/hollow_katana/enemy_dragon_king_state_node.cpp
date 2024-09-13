#include "enemy_dragon_king_state_node.h"
#include "enemy_dragon_king.h"
#include "character_manager.h"
#include "audio_manager.h"



EnemyDragonKingIdleState::EnemyDragonKingIdleState()
{
	timer.set_one_shot(true);
	timer.set_wait_time(1.5);
	timer.set_on_timeout([&]
		{
			// 玩家死亡后,不再跳转状态
			Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
			if (player->get_hp() <= 0)
				return;

			// 策略: 1.半血以上[侧重近战攻击] 2.半血以下[侧重远程攻击] 
			EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
			float distance = abs(enemy->get_position().x - player->get_position().x);
			int rand_num = random_range(0, 100);

			// for test
			if (rand_num <= 50)
				enemy->switch_state("jump");			
			else
				enemy->switch_state("run");				


			//if (enemy->get_hp() >= enemy->get_hp_max() / 2)
			//{
			//	if (rand_num <= 25)
			//		enemy->switch_state("jump");			// 25% 跳跃
			//	else if (rand_num <= 50)
			//		enemy->switch_state("run");				// 25% 奔跑
			//	else if (rand_num <= 80)
			//		enemy->switch_state("squat");			// 30% 下蹲(准备冲刺)
			//	else if (rand_num <= 90 && distance <= MIN_DIS)
			//		enemy->switch_state("throw_silk");		// 10% 丝线攻击
			//	else
			//		enemy->switch_state("throw_sword");	// 10% 扔出剑
			//}
			//else
			//{
			//	if (rand_num <= 25)
			//		enemy->switch_state("jump");			// 25% 跳跃
			//	else if (rand_num <= 45)
			//		enemy->switch_state("throw_barbs");	// 20% 扔出刺球
			//	else if (rand_num <= 55)
			//		enemy->switch_state("squat");			// 10% 下蹲(准备冲刺)
			//	else if (rand_num <= 65 && distance <= MIN_DIS * 1.5f)
			//		enemy->switch_state("throw_silk");		// 10% 丝线攻击
			//	else
			//		enemy->switch_state("throw_sword");	// 35% 扔出剑
			//}
		});
}

void EnemyDragonKingIdleState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("idle");

	EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
	enemy->set_velocity({ 0, 0 });

	float wait_time = 0.15f;
	if (enemy->get_hp() >= enemy->get_hp_max() / 2)
		wait_time += (float)random_range(0, 50) * 0.01f;		// 0.15s + 0.0s ~ 0.5s
	else
		wait_time += (float)random_range(0, 25) * 0.01f;		// 0.15s + 0.0s ~ 0.25s

	timer.set_wait_time(wait_time);
	timer.restart();
}
void EnemyDragonKingIdleState::on_update(float delta)
{
	timer.on_update(delta);

	EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_enemy = enemy->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	float distance = (pos_enemy - pos_player).length();
	enemy->set_facing_left(pos_player.x < pos_enemy.x);

	// [闲置]可跳转状态: 死亡 跳跃 奔跑 普攻 地面攻击波 雷闪一刀
	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->get_velocity().y > 0)
		enemy->switch_state("fall");
}
void EnemyDragonKingIdleState::on_exit()
{
	// 跟新角色朝向
	EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
	Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
	enemy->set_facing_left(player->get_position().x < enemy->get_position().x);
}



void EnemyDragonKingJumpState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("jump");


	EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
	Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
	const Vector2& pos_enemy = enemy->get_position();
	const Vector2& pos_player = player->get_position();
	float distance = abs(pos_enemy.x - pos_player.x);


	// todo: 大跳,小跳,进行缩放			
	// 或者大跳之后可以放[光剑天降], fall 节点通过角色位置高度判断是大跳,小跳,决定招式
	float x_speed = 0.0f;
	int rand_num = random_range(0, 100);
	// 远离玩家
	if (distance <= MIN_DIS)
	{
		x_speed = (pos_enemy.x < pos_player.x ? -SPEED_MOVE_AXIS : SPEED_MOVE_AXIS);
	}
	// 接近玩家
	else
	{
		x_speed = (pos_enemy.x < pos_player.x ? SPEED_MOVE_AXIS : -SPEED_MOVE_AXIS);
	}
	enemy->set_facing_left(x_speed < 0);

	// 大跳
	if (rand_num <= 50)
	{
		enemy->set_big_jump(true);
		enemy->set_velocity({ x_speed, -enemy->get_speed_jump_big() });
	}
	else
	{
		enemy->set_big_jump(false);
		enemy->set_velocity({ x_speed, -enemy->get_speed_jump_small() });
	}
}

void EnemyDragonKingJumpState::on_update(float delta)
{
	EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());

	// [跳跃]可跳转状态: 死亡 下落
	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->get_velocity().y > 0)
	{
		// for test
		int rand_num = random_range(0, 100);
		
		enemy->switch_state("fall");
	}
}



void EnemyDragonKingRunState::on_enter()
{
	CharacterManager::instance()->get_enemy()->set_animation("run");

	AudioManager::instance()->play_audio_ex(_T("enemy_run"), true);
}
void EnemyDragonKingRunState::on_update(float delta)
{
	EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
	const Vector2& pos_enemy = enemy->get_position();
	const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
	enemy->set_facing_left(pos_player.x < pos_enemy.x);
	enemy->set_velocity({ pos_player.x < pos_enemy.x ? -SPEED_RUN : SPEED_RUN, 0 });


	// [奔跑]可跳转状态: 死亡 
	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (abs(pos_player.x - pos_enemy.x) <= MIN_DISTANCE)
	{
		int num = random_range(0, 100);
		enemy->switch_state("idle");			
	}
}
void EnemyDragonKingRunState::on_exit()
{
	CharacterManager::instance()->get_enemy()->set_velocity({ 0, 0 });
	AudioManager::instance()->stop_audio_ex(_T("enemy_run"));
}


void EnemyDragonKingFallState::on_enter()
{
	EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
	if (enemy->get_big_jump())
	{
		enemy->set_animation("fall_big");
	}
	else
	{
		enemy->set_animation("fall_small");
	}
}

void EnemyDragonKingFallState::on_update(float delta)
{
	EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());

	// [下落]可跳转状态: 死亡 闲置
	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->is_on_floor())
		enemy->switch_state("idle");
}
