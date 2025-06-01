#pragma once

#include "singleton.h"
#include "config_manager.h"
#include "coin_prop.h"

class CoinMgr : public Singleton<CoinMgr>
{
	friend class Singleton<CoinMgr>;
	typedef std::vector<CoinProp*> CoinPropList;
public:
	void on_input(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button != SDL_BUTTON_RIGHT) break;			// 鼠标右键拾取金币
			SDL_FPoint cursor_pos = { event.motion.x , event.motion.y };
			for (auto* coin_prop : coin_prop_list)
			{
				SDL_FRect coin_rect = coin_prop->get_rect();
				if (SDL_PointInFRect(&cursor_pos, &coin_rect))
					coin_prop->pick_up();
			}
		}
	}

	void on_update(float delta)
	{
		timer_add_coin.on_update(delta);

		for (auto& coin_prop : coin_prop_list)
			coin_prop->on_update(delta);

		// 移除失效/被拾取
		coin_prop_list.erase(std::remove_if(coin_prop_list.begin(), coin_prop_list.end(),
			[this](CoinProp* coin_prop)
			{
				bool deletable = coin_prop->can_remove();
				if (coin_prop->is_picked_up()) increase_coins(10);
				if(deletable) delete coin_prop;				// debug:if(deletable),不然报错奇怪
				return deletable;
			}), coin_prop_list.end());
	}

	void on_render(SDL_Renderer* renderer)
	{
		for (auto& coin_prop : coin_prop_list)
			coin_prop->on_render(renderer);
	}

	int get_coins_num() const
	{
		return coins;
	}

	void increase_coins(int val)
	{
		coins += val;
		Mix_PlayChannel(-1, ResMgr::instance()->find_sound(ResID::Sound_Coin), 0);
	}

	void decrease_coins(int val)
	{
		coins -= val;
	}

	void create_coin_prop(const Vector2& position)
	{
		CoinProp* coin_prop = new CoinProp(position);
		if (!coin_prop) return;
		coin_prop_list.push_back(coin_prop);
	}

protected:
	CoinMgr()
	{
		coins = ConfigMgr::instance()->init_coin;
		timer_add_coin.set_one_shot(false);
		timer_add_coin.set_wait_time(0.65);
		timer_add_coin.set_on_timeout([this]()
			{
				coins += 1;
			});
	}
	~CoinMgr() = default;

private:
	int coins = 100;				// 当前金币数目
	CoinPropList coin_prop_list;	// 场地上金币道具列表
	Timer timer_add_coin;			// 随着时间增加金币定时器
};
