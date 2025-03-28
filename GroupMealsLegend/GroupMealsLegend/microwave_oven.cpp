#include "microwave_oven.h"

#include "cursor_manager.h"
#include "resources_manager.h"

MicrowaveOven::MicrowaveOven(int x, int y) :Region({ x, y, 284, 176 })
{
	timer.set_one_shot(true);
	timer.set_wait_time(9.0f);
	timer.set_on_timeout([this]
		{
			is_working = false;
		});
}

void MicrowaveOven::on_update(float delta)
{
	if (is_working)
		timer.on_update(delta);
}

// ·ÅÖÃ
void MicrowaveOven::on_cursor_up()
{
	if (is_working)
		return;

	if (meal_target == Meal::None && can_place(CursorMgr::instance()->get_picked()))
	{
		is_working = true;
		timer.restart();
		meal_target = CursorMgr::instance()->get_picked();
		CursorMgr::instance()->set_picked(Meal::None);
	}
}

// È¡³ö
void MicrowaveOven::on_cursor_down()
{
	if (is_working || CursorMgr::instance()->get_picked() != Meal::None)
		return;

	switch (meal_target)
	{
	case Meal::BraisedChicken_Cold: meal_target = Meal::BraisedChicken_Hot; break;
	case Meal::MeatBall_Cold: meal_target = Meal::MeatBall_Hot; break;
	case Meal::RedCookedPork_Cold: meal_target = Meal::RedCookedPork_Hot; break;
	}
	CursorMgr::instance()->set_picked(meal_target);
	meal_target = Meal::None;
}

void MicrowaveOven::on_render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, ResMgr::instance()->find_texture(is_working ? "mo_working" : "mo_open"), nullptr, &get_rect());

	if (!is_working)
	{
		SDL_Texture* texture = nullptr;
		switch (meal_target)
		{
		case Meal::BraisedChicken_Hot: texture = ResMgr::instance()->find_texture("bc_hot"); break;
		case Meal::MeatBall_Hot: texture = ResMgr::instance()->find_texture("mb_hot"); break;
		case Meal::RedCookedPork_Hot: texture = ResMgr::instance()->find_texture("rcp_hot"); break;
		}
		SDL_Rect rect_meal = { _rect.x + 113, _rect.y + 65, 0, 0 };
		SDL_QueryTexture(texture, nullptr, nullptr, &rect_meal.w, &rect_meal.h);
		SDL_RenderCopy(renderer, texture, nullptr, &rect_meal);
	}
}

bool MicrowaveOven::can_place(Meal meal)
{
	return meal == Meal::BraisedChicken_Cold || meal == Meal::MeatBall_Cold || meal == Meal::RedCookedPork_Cold;
}