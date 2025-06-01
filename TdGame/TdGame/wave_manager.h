#pragma once

#include "singleton.h"
#include "config_manager.h"
#include "enemy_manager.h"
#include "coin_manager.h"
#include "timer.h"

class WaveMgr : public Singleton<WaveMgr>
{
	friend class Singleton<WaveMgr>;

public:
	WaveMgr()
	{
		const WaveList& wave_list = ConfigMgr::instance()->wave_list;
		timer_wave.set_one_shot(true);
		timer_wave.set_wait_time(wave_list[wave_idx].interval);
		timer_wave.set_on_timeout([this]()
			{
				wave_start();
			});

		timer_spawn.pause();
		timer_spawn.set_one_shot(true);
		timer_spawn.set_on_timeout([this]()
			{
				spawn_enemy();
			});

	}
	~WaveMgr() = default;

	void on_update(float delta)
	{
		if (wave_finished) return;

		timer_wave.on_update(delta);
		timer_spawn.on_update(delta);
		check_wave_finished();
	}

	bool is_wave_finished() const
	{
		return wave_finished;
	}

private:
	void wave_start()
	{
		const WaveList& wave_list = ConfigMgr::instance()->wave_list;
		timer_spawn.set_wait_time(wave_list[wave_idx].spawn_list[spawn_idx].interval);
		timer_spawn.restart();
	}

	void spawn_enemy()
	{
		const WaveList& wave_list = ConfigMgr::instance()->wave_list;
		const Wave::SpawnList& spawn_list = wave_list[wave_idx].spawn_list;

		if (spawn_idx >= spawn_list.size()) return;
		EnemyMgr::instance()->create_enemy(spawn_list[spawn_idx].enemy, spawn_list[spawn_idx].point);
		spawn_idx++;
		if(spawn_idx < spawn_list.size())
		{
			timer_spawn.set_wait_time(spawn_list[spawn_idx].interval);
			timer_spawn.restart();
		}
	}

	void check_wave_finished()
	{
		if (wave_finished) return;

		const WaveList& wave_list = ConfigMgr::instance()->wave_list;
		const Wave::SpawnList& spawn_list = wave_list[wave_idx].spawn_list;
		if (EnemyMgr::instance()->get_enemy_list().empty() && spawn_idx >= spawn_list.size())
		{
			// 波次结束奖励
			CoinMgr::instance()->increase_coins(wave_list[wave_idx].rewards);

			wave_idx++;
			spawn_idx = 0;
			if (wave_idx >= wave_list.size())
			{
				wave_finished = true;
				Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_Win), 0);
				return;
			}
			timer_wave.set_wait_time(wave_list[wave_idx].interval);
			timer_wave.restart();
		}
	}

private:
	Timer timer_wave;
	Timer timer_spawn;
	bool wave_finished = false;			// 所有波次结束，当前波次刷新敌人全部死亡
	int wave_idx = 0;					// 当前波次index
	int spawn_idx = 0;					// 当前波次下敌人生成列表index

};
