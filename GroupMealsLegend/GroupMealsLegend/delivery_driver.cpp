#include "delivery_driver.h"

DeliveryDriver::DeliveryDriver(int x, int y):Region({x, y, 192, 272})
{
	refresh_timer.set_one_shot(true);
	refresh_timer.set_wait_time(15);
	refresh_timer.set_on_timeout([this]
		{
			refresh();
		});
}

void DeliveryDriver::refresh()
{
	static Meal food_list[] = { Meal::BraisedChicken_Hot, Meal::MeatBall_Hot, Meal::RedCookedPork_Hot };
	static Meal drink_list[] = { Meal::Cola, Meal::Sprite };
	is_waiting = true;
	is_meituan = rand() % 2;
	int food_count = rand() % 3 + 1;
	for (int i = 0; i < food_count; ++i)
	{
		list.push_back({ food_list[rand() % (sizeof(food_list) / sizeof(food_list[0]))], false });
	}
	int drink_limit = 6;
	switch (food_count)
	{
	case 1: drink_limit = 6; break;
	case 2: drink_limit = 4; break;
	case 3: drink_limit = 2; break;
	default: break;
	}
	int drink_count = rand() % drink_limit + 1;
	for (int i = 0; i < drink_count; ++i)
	{
		list.push_back({ drink_list[rand() % (sizeof(drink_list) / sizeof(drink_list[0]))], false });
	}

	int rand_time = rand() % 7;
	wait_time = 30.0f + (rand() % 2 ? rand_time : -rand_time);
}

void DeliveryDriver::on_update(float delta)
{
	if (is_waiting)
	{
		pass_time += delta;
	}
	else
	{
		refresh_timer.on_update(delta);
	}
}

void DeliveryDriver::on_render(SDL_Renderer* renderer)
{

}