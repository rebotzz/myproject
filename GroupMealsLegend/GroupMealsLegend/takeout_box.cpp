#include "takeout_box.h"

#include "resources_manager.h"
#include "cursor_manager.h"

TakeoutBox::TakeoutBox(int x, int y) :Region({ x, y, 92, 48 + 15 })
{
	static Meal list[] = { Meal::TakeoutBox, Meal::BraisedChicken_Cold, Meal::BraisedChicken_Hot,
	Meal::MeatBall_Cold, Meal::MeatBall_Hot, Meal::RedCookedPork_Cold, Meal::RedCookedPork_Hot };
	for(auto id : list)
		can_place_list.insert(id);
};

void TakeoutBox::on_render(SDL_Renderer* renderer)
{
	SDL_Texture* texture = nullptr;
	switch (meal)
	{
	case Meal::TakeoutBox: texture = ResMgr::instance()->find_texture("tb"); break;
	case Meal::BraisedChicken_Cold: texture = ResMgr::instance()->find_texture("bc_cold"); break;
	case Meal::BraisedChicken_Hot: texture = ResMgr::instance()->find_texture("bc_hot"); break;
	case Meal::MeatBall_Cold: texture = ResMgr::instance()->find_texture("mb_cold"); break;
	case Meal::MeatBall_Hot: texture = ResMgr::instance()->find_texture("mb_hot"); break;
	case Meal::RedCookedPork_Cold: texture = ResMgr::instance()->find_texture("rcp_cold"); break;
	case Meal::RedCookedPork_Hot: texture = ResMgr::instance()->find_texture("rcp_hot"); break;
	default: break;
	}

	if (texture)
	{
		SDL_Rect rect = { get_rect().x, get_rect().y, 0, 0 };
		SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}
}

// ×¥È¡
void TakeoutBox::on_cursor_down()
{
	if (CursorMgr::instance()->get_picked() == Meal::None)
	{
		CursorMgr::instance()->set_picked(meal);
		meal = Meal::None;
	}

}

// ·ÅÖÃ
void TakeoutBox::on_cursor_up()
{
	if (meal == Meal::None && can_place(CursorMgr::instance()->get_picked()))
	{
		meal = CursorMgr::instance()->get_picked();
		CursorMgr::instance()->set_picked(Meal::None);
	}
	else if (meal == Meal::TakeoutBox)
	{
		switch (CursorMgr::instance()->get_picked())
		{
		case Meal::BraisedChicken_Box:
			meal = Meal::BraisedChicken_Cold;
			CursorMgr::instance()->set_picked(Meal::None);
			break;
		case Meal::MeatBall_Box:
			meal = Meal::MeatBall_Cold;
			CursorMgr::instance()->set_picked(Meal::None);
			break;
		case Meal::RedCookedPork_Box:
			meal = Meal::RedCookedPork_Cold;
			CursorMgr::instance()->set_picked(Meal::None);
			break;
		default:
			break;
		}
	}
}

bool TakeoutBox::can_place(Meal id)
{
	return can_place_list.count(id);
}