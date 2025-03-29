#include "delivery_driver.h"

#include "resources_manager.h"
#include "cursor_manager.h"

DeliveryDriver::DeliveryDriver(int x, int y):Region({x, y, 192, 272})
{
	timer_refresh.set_one_shot(true);
	timer_refresh.set_wait_time(7);
	timer_refresh.set_on_timeout([this]
		{
			refresh();
		});
	//refresh();
	//is_waiting = rand() % 2;
}

void DeliveryDriver::refresh()
{
	// 重置状态
	is_waiting = true;
	waited_time = 0;
	list.clear();
	timer_refresh.restart();

	// 随机刷新时间,等待时间
	int rand_time1 = rand() % 7, rand_time2 = rand() % 15;
	waited_time_limit = 40.0f + (rand() % 2 ? rand_time1 : -rand_time1);
	timer_refresh.set_wait_time(12 + rand_time2);
	// 随机外卖员
	is_meituan = rand() % 2;

	// 随机餐品
	static Meal food_list[] = { Meal::BraisedChicken_Hot, Meal::MeatBall_Hot, Meal::RedCookedPork_Hot };
	static Meal drink_list[] = { Meal::Cola, Meal::Sprite };
	int food_count = rand() % 4;
	for (int i = 0; i < food_count; ++i)
	{
		list.push_back({ food_list[rand() % (sizeof(food_list) / sizeof(food_list[0]))], true });
	}
	int drink_limit = 6;
	switch (food_count)
	{
	case 0: drink_limit = 8; break;
	case 1: drink_limit = 6; break;
	case 2: drink_limit = 4; break;
	case 3: drink_limit = 2; break;
	default: break;
	}
	int drink_count = rand() % drink_limit + 1;
	for (int i = 0; i < drink_count; ++i)
	{
		list.push_back({ drink_list[rand() % (sizeof(drink_list) / sizeof(drink_list[0]))], true });
	}

	Mix_PlayChannel(-1, ResMgr::instance()->find_audio("get_list"), 0);
}

void DeliveryDriver::on_update(float delta)
{
	// 等待中
	if (is_waiting)
	{
		// 等待超时
		waited_time += delta;
		if (waited_time >= waited_time_limit)
		{
			is_waiting = false;
			return;
		}

		// 交付完成
		bool complete = true;
		for (auto& [meal, need] : list)
		{
			if (need)
			{
				complete = false;
				break;
			}
		}
		if (complete)
		{
			is_waiting = false;
			Mix_PlayChannel(-1, ResMgr::instance()->find_audio("complete"), 0);
			CursorMgr::instance()->add_coins(list.size() * 2);
		}
	}
	// 没有外卖员等待，更新外卖员刷新计时器
	else
	{
		timer_refresh.on_update(delta);
	}
}

void DeliveryDriver::on_render(SDL_Renderer* renderer)
{
	if (!is_waiting) return;

	// 外卖员
	SDL_RenderCopy(renderer, ResMgr::instance()->find_texture(is_meituan ? "meituan" : "eleme"), nullptr, &_rect);

	// 耐心条
	SDL_Rect rect_border = { _rect.x - 35, _rect.y + 15, 40, 160 };
	int h = 160 * (waited_time / waited_time_limit);
	SDL_Rect rect_content_src = { 0, 160 - h, rect_border.w, h };
	SDL_Rect rect_content_dst = { rect_border.x, rect_border.y + 160 - h, rect_border.w, h };
	static SDL_Texture* tex_patience_border = ResMgr::instance()->find_texture("patience_border");
	static SDL_Texture* tex_patience_content = ResMgr::instance()->find_texture("patience_content");
	SDL_RenderCopy(renderer, tex_patience_border, nullptr, &rect_border);
	SDL_RenderCopy(renderer, tex_patience_content, &rect_content_src, &rect_content_dst);

	// 需求框
	SDL_Rect rect_bubble = { _rect.x + 175, _rect.y + 55, 76, 139 };
	static SDL_Texture* tex_bubble = ResMgr::instance()->find_texture("bubble");
	SDL_RenderCopy(renderer, tex_bubble, nullptr, &rect_bubble);

	// 需要餐品
	int row = 0, col = 0;
	for (auto& [meal, need] : list)
	{
		SDL_Texture* tex_meal = nullptr;
		SDL_Rect rect_meal = {rect_bubble.x + 15 + col * 32, rect_bubble.y + 3 + row * 34, 0, 0 };

		switch (meal)
		{
		case Meal::BraisedChicken_Hot: tex_meal = ResMgr::instance()->find_texture("bc_icon"); row++; break;
		case Meal::MeatBall_Hot: tex_meal = ResMgr::instance()->find_texture("mb_icon"); row++;  break;
		case Meal::RedCookedPork_Hot: tex_meal = ResMgr::instance()->find_texture("rcp_icon"); row++;  break;
		case Meal::Cola: tex_meal = ResMgr::instance()->find_texture("cola_icon"); col++; if (col == 2) row++; col %= 2; break;
		case Meal::Sprite: tex_meal = ResMgr::instance()->find_texture("sprite_icon"); col++; if (col == 2) row++; col %= 2; break;
		default: break;
		}
		SDL_QueryTexture(tex_meal, nullptr, nullptr, &rect_meal.w, &rect_meal.h);

		if (!need)
		{
			SDL_SetTextureAlphaMod(tex_meal, 125);	// 不透明度0~255
		}
		else
		{
			SDL_SetTextureAlphaMod(tex_meal, 255);	
		}
		SDL_RenderCopy(renderer, tex_meal, nullptr, &rect_meal);
	}
}

// 交付餐品
void DeliveryDriver::on_cursor_up()
{
	if (!is_waiting)
		return;
	if (CursorMgr::instance()->get_picked() == Meal::None)
		return;
	for (auto& [meal, need] : list)
	{
		if (need && CursorMgr::instance()->get_picked() == meal)
		{
			need = false;
			CursorMgr::instance()->set_picked(Meal::None);
			Mix_PlayChannel(-1, ResMgr::instance()->find_audio("complete"), 0);
			break;
		}
	}
}

