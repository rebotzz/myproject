#include "enemy_dragon_king_state_node.h"
#include "enemy_dragon_king.h"
#include "character_manager.h"
#include "audio_manager.h"
#include "particle_manager.h"
#include "scene_manager.h"
#include "effect.h"


namespace EnemyDragonKingState
{

	IdleState::IdleState()
	{
		timer.set_one_shot(true);
		timer.set_wait_time(1.5);
		timer.set_on_timeout([&]
			{
				// 玩家死亡后,不再跳转状态
				Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
				if (player->get_hp() <= 0)
					return;

				// 策略: 龙王擅长近战,习惯追击玩家,血量低时加入天空火焰
				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				float distance = abs(enemy->get_position().x - player->get_position().x);

				if (enemy->get_hp() == enemy->get_hp_max() / 2 && is_first_half_hp)
				{
					is_first_half_hp = false;
					enemy->switch_state("jump");
					return;
				}

				int rand_num = random_range(0, 100);
				if (enemy->get_hp() >= enemy->get_hp_max() / 2)
				{
					if (distance >= 500.f)
					{
						if(rand_num <= 50)
							enemy->switch_state("run");
						else
							enemy->switch_state("fire_dash");
					}
					else if (rand_num <= 95)
						enemy->switch_state("prepare");
					else
						enemy->switch_state("jump");
				}
				else
				{
					if (distance >= 500.f)
					{
						if (rand_num <= 40)
							enemy->switch_state("run");
						else
							enemy->switch_state("fire_dash");
					}
					else if (rand_num <= 90)
						enemy->switch_state("prepare");
					else
						enemy->switch_state("jump");
				}
			});
	}
	void IdleState::on_enter()
	{
		CharacterManager::instance()->get_enemy()->set_animation("idle");

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		enemy->set_velocity({ 0, 0 });

		float wait_time = 0.25f;
		if (enemy->get_hp() >= enemy->get_hp_max() / 2)
			wait_time += (float)random_range(0, 50) * 0.01f;
		else
			wait_time += (float)random_range(0, 25) * 0.01f;

		timer.set_wait_time(wait_time);
		timer.restart();
	}
	void IdleState::on_update(float delta)
	{
		timer.on_update(delta);

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		const Vector2& pos_enemy = enemy->get_position();
		const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
		enemy->set_facing_left(pos_player.x < pos_enemy.x);

		// [闲置]可跳转状态: 死亡 跳跃 奔跑 普攻 地面攻击波 雷闪一刀
		if (enemy->get_hp() <= 0)
			enemy->switch_state("dead");
		else if (enemy->get_velocity().y > 0)
			enemy->switch_state("fall");
	}
	void IdleState::on_exit()
	{
		// 跟新角色朝向
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
		enemy->set_facing_left(player->get_position().x < enemy->get_position().x);
	}


	void JumpState::on_enter()
	{
		CharacterManager::instance()->get_enemy()->set_animation("jump");

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
		const Vector2& pos_enemy = enemy->get_position();
		const Vector2& pos_player = player->get_position();
		enemy->set_facing_left(pos_player.x < pos_enemy.x);

		// todo: 大跳,小跳,进行缩放			
		// 或者大跳之后可以放[光剑天降], fall 节点通过角色位置高度判断是大跳,小跳,决定招式
		int rand_num = random_range(0, 100);
		enemy->set_velocity({ pos_player.x < pos_enemy.x ? -SPEED_MOVE : SPEED_MOVE, -enemy->get_speed_jump() });

		AudioManager::instance()->play_audio_ex(_T("jump"));

		std::shared_ptr<EffectJump> particle = std::make_shared<EffectJump>();
		particle->set_position(enemy->get_position());
		ParticleManager::instance()->register_particle(particle);
	}

	void JumpState::on_update(float delta)
	{
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());

		// [跳跃]可跳转状态: 死亡 下落
		if (enemy->get_hp() <= 0)
			enemy->switch_state("dead");
		else if (enemy->get_velocity().y > 0)
			enemy->switch_state("fire_bullet");
	}

	void RunState::on_enter()
	{
		CharacterManager::instance()->get_enemy()->set_animation("run");
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
		enemy->set_velocity({ pos_player.x < enemy->get_position().x ? -SPEED_RUN : SPEED_RUN, 0 });
		enemy->set_facing_left(pos_player.x < enemy->get_position().x);

		AudioManager::instance()->play_audio_ex(_T("run_loop"), true);
	}
	void RunState::on_update(float delta)
	{
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		const Vector2& pos_enemy = enemy->get_position();
		const Vector2& pos_player = CharacterManager::instance()->get_player()->get_position();
		float distance = abs(pos_player.x - pos_enemy.x);

		// [奔跑]可跳转状态: 死亡 
		if (enemy->get_hp() <= 0)
			enemy->switch_state("dead");
		else if (distance <= 400.f)
		{
			int rand_num = random_range(0, 100);
			if (enemy->get_hp() >= enemy->get_hp_max() / 2)
			{
				if (rand_num <= 10)
					next_state = "fire_dash";
				else
				{
					std::shared_ptr<EffectLeaves> particle = std::make_shared<EffectLeaves>();
					particle->set_position(enemy->get_logic_center());
					ParticleManager::instance()->register_particle(particle);
					AudioManager::instance()->play_audio_ex(_T("wind"));

					enemy->switch_state("normal_attack");
				}
			}
			else
			{
				if (rand_num <= 15)
					enemy->switch_state("fire_dash");
				else if (rand_num <= 60)
				{
					std::shared_ptr<EffectLeaves> particle = std::make_shared<EffectLeaves>();
					particle->set_position(enemy->get_logic_center());
					ParticleManager::instance()->register_particle(particle);
					AudioManager::instance()->play_audio_ex(_T("wind"));

					enemy->switch_state("normal_attack");
				}
				else if (rand_num <= 80)
				{
					std::shared_ptr<EffectElectric> particle = std::make_shared<EffectElectric>();
					particle->set_position(enemy->get_logic_center() + Vector2(-5, -20));
					ParticleManager::instance()->register_particle(particle);
					AudioManager::instance()->play_audio_ex(_T("electric"));

					enemy->switch_state("electric");
				}
				else
					next_state = "jump";
			}
		}

	}
	void RunState::on_exit()
	{
		CharacterManager::instance()->get_enemy()->set_velocity({ 0, 0 });
		AudioManager::instance()->stop_audio_ex(_T("run_loop"));
	}


	void FallState::on_enter()
	{
		CharacterManager::instance()->get_enemy()->set_animation("fall");
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		// 加入微小的速度
		enemy->set_velocity({ enemy->get_facing_left() ? -150.f : 150.f , 0 });

		AudioManager::instance()->play_audio_ex(_T("fall"));
	}

	void FallState::on_update(float delta)
	{
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());

		// [下落]可跳转状态: 死亡 闲置
		if (enemy->get_hp() <= 0)
			enemy->switch_state("dead");
		else if (enemy->is_on_floor())
		{
			enemy->switch_state("idle");

			AudioManager::instance()->play_audio_ex(_T("land"));
			std::shared_ptr<EffectLand> particle = std::make_shared<EffectLand>();
			particle->set_position(enemy->get_position());
			ParticleManager::instance()->register_particle(particle);
		}
	}

	PrepareState::PrepareState()
	{
		timer_prepare.set_one_shot(true);
		timer_prepare.set_wait_time(0.4f);
		timer_prepare.set_on_timeout([&]
			{
				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				enemy->switch_state(next_state);
			});

	}
	void PrepareState::on_enter()
	{
		// todo: 新加动画 prepare, 即idle动画截取,停留在最后一帧,出刀准备
		CharacterManager::instance()->get_enemy()->set_animation("prepare");

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		const auto& pos_player = CharacterManager::instance()->get_player()->get_position();
		float distance = abs(pos_player.x - enemy->get_position().x);
		float wait_time = 0.0f;
		int rand_num = random_range(0, 100);
		// 初步方案,之后加入距离,血量判断
		if (enemy->get_hp() > enemy->get_hp_max() / 2)
		{
			if (rand_num <= 60 && distance <= 400.f)
			{
				// 生成特效,标识出招前摇
				next_state = "normal_attack";
				wait_time = 0.3f + random_range(0, 15) * 0.01f;
				timer_effect.set_wait_time(1.0f);

				std::shared_ptr<EffectLeaves> particle = std::make_shared<EffectLeaves>();
				particle->set_position(enemy->get_logic_center());
				ParticleManager::instance()->register_particle(particle);
				AudioManager::instance()->play_audio_ex(_T("wind"));
			}
			else
			{
				next_state = "electric";
				wait_time += 0.6f + random_range(0, 15) * 0.01f;
				timer_effect.set_wait_time(0.1f);

				std::shared_ptr<EffectElectric> particle = std::make_shared<EffectElectric>();
				particle->set_position(enemy->get_logic_center() + Vector2(-5, -20));
				ParticleManager::instance()->register_particle(particle);
				AudioManager::instance()->play_audio_ex(_T("electric"));
			}
		}
		else
		{
			if (distance <= 400.f)
			{
				next_state = "normal_attack";
				wait_time = 0.25f + random_range(0, 15) * 0.01f;
				timer_effect.set_wait_time(1.0f);

				std::shared_ptr<EffectLeaves> particle = std::make_shared<EffectLeaves>();
				particle->set_position(enemy->get_logic_center());
				ParticleManager::instance()->register_particle(particle);
				AudioManager::instance()->play_audio_ex(_T("wind"));
			}
			else
			{
				next_state = "electric";
				wait_time += 0.5f + random_range(0, 10) * 0.01f;
				timer_effect.set_wait_time(0.1f);

				std::shared_ptr<EffectElectric> particle = std::make_shared<EffectElectric>();
				particle->set_position(enemy->get_logic_center() + Vector2(-5, -20));
				ParticleManager::instance()->register_particle(particle);
				AudioManager::instance()->play_audio_ex(_T("electric"));
			}
		}

		timer_prepare.restart();
		timer_prepare.set_wait_time(wait_time);
	}
	void PrepareState::on_update(float delta)
	{
		timer_prepare.on_update(delta);

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		if (enemy->get_hp() <= 0)
			enemy->switch_state("dead");
	}

	// [普通一刀]
	AttackNormalState::AttackNormalState()
	{
		timer_duration.set_one_shot(true);
		timer_duration.set_wait_time(0.07f * 5);
		timer_duration.set_on_timeout([&]
			{
				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				enemy->set_attacking(false);
				enemy->set_velocity({ enemy->get_facing_left() ? -SPEED_MOVE_AXIS * 0.01f : SPEED_MOVE_AXIS * 0.01f, 0 });
			});

		timer_attack.set_one_shot(true);
		timer_attack.set_wait_time(0.075f * 2);
		timer_attack.set_on_timeout([&]
			{
				is_start = true;

				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				enemy->set_attacking(true);
				enemy->on_attack();
				enemy->update_attack_box_position();
			});

		timer_exit.set_one_shot(true);
		timer_exit.set_wait_time(0.075f * 9);
		timer_exit.set_on_timeout([&]
			{
				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				enemy->switch_state("idle");
			});
	}
	void AttackNormalState::on_enter()
	{
		CharacterManager::instance()->get_enemy()->set_animation("attack");
		timer_attack.restart();
		timer_exit.restart();
		timer_duration.restart();
		is_start = false;

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		Character* player = dynamic_cast<Character*>(CharacterManager::instance()->get_player());
		bool is_left = player->get_position().x < enemy->get_position().x;
		enemy->set_facing_left(is_left);
		enemy->set_velocity({ is_left ? -SPEED_MOVE_AXIS : SPEED_MOVE_AXIS, 0 });
		enemy->set_attack_dir(is_left ? EnemyDragonKing::Direction::Left : EnemyDragonKing::Direction::Right);

	}
	void AttackNormalState::on_update(float delta)
	{
		timer_attack.on_update(delta);
		if(is_start)
			timer_duration.on_update(delta);
		timer_exit.on_update(delta);

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		enemy->update_attack_box_position();

		if (enemy->get_hp() <= 0)
			enemy->switch_state("dead");
	}

	// [雷闪一刀]
	ElectricState::ElectricState()
	{
		timer_duration.set_one_shot(true);
		timer_duration.set_wait_time(0.075f * 9);
		timer_duration.set_on_timeout([]
			{
				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				enemy->switch_state("idle");
				enemy->set_attacking(false);
			});

		timer_slow.set_one_shot(true);
		timer_slow.set_wait_time(0.075f * 6);
		timer_slow.set_on_timeout([]
			{
				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				enemy->set_velocity({ enemy->get_facing_left() ? -400.f : 400.f, 0 });
				enemy->switch_state("idle");
				enemy->set_attacking(false);
			});

		timer_effect_line.set_one_shot(false);
		timer_effect_line.set_wait_time(0.15f);
		timer_effect_line.set_on_timeout([]
			{
				// 生成经过路径的尾迹线特效
				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				std::shared_ptr<EffectDashLine> particle(new EffectDashLine(enemy->get_facing_left()));
				particle->set_position(enemy->get_position() + Vector2(0.f, -40.f));
				ParticleManager::instance()->register_particle(particle);

				std::shared_ptr<EffectElectricAxis> electric(new EffectElectricAxis(enemy->get_facing_left()));
				electric->set_position(enemy->get_position() + Vector2(0.f, -40.f));
				ParticleManager::instance()->register_particle(electric);
			});
	}
	void ElectricState::on_enter()
	{
		CharacterManager::instance()->get_enemy()->set_animation("attack");
		timer_duration.restart();
		timer_effect_line.restart();
		timer_slow.restart();

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		enemy->set_velocity({ enemy->get_facing_left() ? -950.f : 950.f, 0 });
		enemy->set_attacking(true);
		enemy->on_attack();

	}
	void ElectricState::on_update(float delta)
	{
		timer_duration.on_update(delta);
		timer_effect_line.on_update(delta);
		timer_slow.on_update(delta);

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		if (enemy->get_hp() <= 0)
			enemy->switch_state("dead");
	}
	void ElectricState::on_exit()
	{}


	// [火焰冲弹]
	FireDashState::FireDashState()
	{
		timer_over.set_one_shot(true);
		timer_over.set_wait_time(0.08f);
		timer_over.set_on_timeout([]
			{
				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				enemy->set_velocity({ enemy->get_facing_left() ? -400.f : 400.f, 0 });
				enemy->set_fire_dash(false);
				enemy->set_invisible(false);
			});

		timer_exit.set_one_shot(true);
		timer_exit.set_wait_time(0.15f);
		timer_exit.set_on_timeout([]
			{
				EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
				int rand_num = random_range(0, 100);
				if (enemy->get_hp() >= enemy->get_hp_max() / 2)
				{
					if (rand_num <= 70)
						enemy->switch_state("idle");
					else
					{
						enemy->set_velocity({ 0, 0 });
						enemy->switch_state("prepare");
					}
				}
				else
				{
					if (rand_num <= 30)
						enemy->switch_state("idle");
					else
					{
						enemy->set_velocity({ 0, 0 });
						enemy->switch_state("prepare");
					}
				}
			});

		timer_max_time.set_one_shot(true);
		timer_max_time.set_wait_time(1.3f);
		timer_max_time.set_on_timeout([&]
			{
				can_finish = true;
			});

		timer_min_time.set_one_shot(true);
		timer_min_time.set_wait_time(0.3f);
		timer_min_time.set_on_timeout([&]
			{
				min_can_finish = true;
			});
	}
	void FireDashState::on_enter()
	{
		// 只有run state 才能进入,所以不用设置动画
		min_can_finish = false;
		can_finish = false;
		timer_over.restart();
		timer_exit.restart();
		timer_max_time.restart();
		timer_min_time.restart();

		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		enemy->set_velocity({ enemy->get_facing_left() ? -550.f : 550.f, 0 });
		enemy->on_fire_dash();
		enemy->set_invisible(true);
		enemy->get_hurt_box()->set_enabled(false);

		AudioManager::instance()->play_audio_ex(_T("fire_loop"));
	}
	void FireDashState::on_update(float delta)
	{
		if (can_finish)
		{
			timer_exit.on_update(delta);
			timer_over.on_update(delta);
		}
		timer_max_time.on_update(delta);
		timer_min_time.on_update(delta);

		// 一直冲刺,直到路过玩家
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		Character* player = CharacterManager::instance()->get_player();
		float distance = abs(enemy->get_position().x - player->get_position().x);
		if (distance <= 50 && min_can_finish)
			can_finish = true;

		if (enemy->get_hp() <= 0)
			enemy->switch_state("dead");
	}
	void FireDashState::on_exit()
	{
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		enemy->set_invisible(false);
		enemy->set_fire_dash(false);
		enemy->get_hurt_box()->set_enabled(true);
	}


	FireBulletState::FireBulletState()
	{
	}
	void FireBulletState::on_enter()
	{
		// 这里不要动画了,加一个特效就行
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		enemy->on_fire_bullet();

		AudioManager::instance()->play_audio_ex(_T("fire_loop"));
	}
	void FireBulletState::on_update(float delta)
	{
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());

		if (enemy->get_hp() <= 0)
			enemy->switch_state("dead");
		else if(enemy->get_velocity().y > 0)
			enemy->switch_state("fall");
	}
	void FireBulletState::on_exit()
	{
	}


	DeadState::DeadState()
	{
		timer_text1.set_one_shot(true);
		timer_text1.set_wait_time(4.f);
		timer_text1.set_on_timeout([]()
			{
				std::shared_ptr<EffectText> particle(new EffectText(
					_T("零号...歧途之心..."), 4.0f, RGB(50, 255, 255)));
				particle->set_position({ (float)getwidth() / 2, (float)getheight() / 2 });
				ParticleManager::instance()->register_particle(particle);
			});

		timer_text2.set_one_shot(true);
		timer_text2.set_wait_time(8.5f);
		timer_text2.set_on_timeout([]()
			{
				std::shared_ptr<EffectText> text1(new EffectText(
					_T("醒了吗...?"), 5.0f, RGB(255, 255, 255)));
				text1->set_position({ (float)getwidth() / 2, (float)getheight() / 2 });
				text1->set_text_offset(0, -20);
				std::shared_ptr<EffectText> text2(new EffectText(
					_T("让我们去把这座城烧成灰吧..."), 5.0f, RGB(255, 255, 255)));
				text2->set_position({ (float)getwidth() / 2, (float)getheight() / 2 });
				text2->set_text_offset(0, 20);
				text2->set_enable_background(false);
				ParticleManager::instance()->register_particle(text1);
				ParticleManager::instance()->register_particle(text2);
			});
	}

	void DeadState::on_enter()
	{
		EnemyDragonKing* enemy = dynamic_cast<EnemyDragonKing*>(CharacterManager::instance()->get_enemy());
		enemy->set_animation("idle");
		enemy->set_velocity({ 0,0 });
		enemy->set_facing_left(CharacterManager::instance()->get_player()->get_position().x < enemy->get_position().x);
		timer_text1.restart();
		timer_text2.restart();

		std::shared_ptr<EffectText> text(new EffectText(
			_T("很好...这样能行"), 3.f, RGB(0, 255, 255)));
		text->set_position({ (float)getwidth() / 2, (float)getheight() / 2 });
		ParticleManager::instance()->register_particle(text);
	}
	void DeadState::on_update(float delta)
	{
		timer_text1.on_update(delta);
		timer_text2.on_update(delta);
	}
}
