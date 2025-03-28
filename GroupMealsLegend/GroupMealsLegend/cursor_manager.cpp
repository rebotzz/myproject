#include "cursor_manager.h"
#include "resources_manager.h"

CursorMgr* CursorMgr::manager = nullptr;
CursorMgr* CursorMgr::instance()
{
	if (nullptr == manager)
	{
		manager = new CursorMgr;
	}
	return manager;
}
void CursorMgr::input(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		pos_cursor.x = event.motion.x;
		pos_cursor.y = event.motion.y;
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			is_mouse_lbtn_down = true;
			switch (rand() % 3)
			{
			case 0: Mix_PlayChannel(-1, ResMgr::instance()->find_audio("click_1"), 0); break;
			case 1: Mix_PlayChannel(-1, ResMgr::instance()->find_audio("click_2"), 0); break;
			case 2: Mix_PlayChannel(-1, ResMgr::instance()->find_audio("click_3"), 0); break;
			default: break;
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			is_mouse_lbtn_down = false;
		}
		break;
	}
}
void CursorMgr::render(SDL_Renderer* renderer)
{
	// 绘制持有的餐品纹理
	SDL_Texture* texture_picked = nullptr;
	switch (meal_picked)
	{
	case Meal::None: break;
	case Meal::Cola:				texture_picked = ResMgr::instance()->find_texture("cola"); break;
	case Meal::Sprite:				texture_picked = ResMgr::instance()->find_texture("sprite"); break;
	case Meal::BraisedChicken_Hot:	texture_picked = ResMgr::instance()->find_texture("bc_hot"); break;
	case Meal::BraisedChicken_Cold:	texture_picked = ResMgr::instance()->find_texture("bc_cold"); break;
	case Meal::MeatBall_Hot:		texture_picked = ResMgr::instance()->find_texture("mb_hot"); break;
	case Meal::MeatBall_Cold:		texture_picked = ResMgr::instance()->find_texture("mb_cold"); break;
	case Meal::RedCookedPork_Hot:	texture_picked = ResMgr::instance()->find_texture("rcp_hot"); break;
	case Meal::RedCookedPork_Cold:	texture_picked = ResMgr::instance()->find_texture("rcp_cold"); break;
	case Meal::BraisedChicken_Box:	texture_picked = ResMgr::instance()->find_texture("bc_box"); break;
	case Meal::MeatBall_Box:		texture_picked = ResMgr::instance()->find_texture("mb_box"); break;
	case Meal::RedCookedPork_Box:	texture_picked = ResMgr::instance()->find_texture("rcp_box"); break;
	case Meal::TakeoutBox:			texture_picked = ResMgr::instance()->find_texture("tb_picked"); break;			
	default: break;
	}

	if (is_mouse_lbtn_down && texture_picked)
	{
		SDL_Rect rect_picked = { pos_cursor.x, pos_cursor.y, 0, 0 };
		SDL_QueryTexture(texture_picked, nullptr, nullptr, &rect_picked.w, &rect_picked.h);
		SDL_RenderCopy(renderer, texture_picked, nullptr, &rect_picked);
	}

	// 绘制光标纹理
	SDL_Rect rect_cursor = { pos_cursor.x ,pos_cursor.y, 0, 0 };
	SDL_Texture* tex_cursor = ResMgr::instance()->find_texture(is_mouse_lbtn_down ? "cursor_down" : "cursor_idle");
	SDL_QueryTexture(tex_cursor, nullptr, nullptr, &rect_cursor.w, &rect_cursor.h);
	SDL_RenderCopy(renderer, tex_cursor, nullptr, &rect_cursor);
}
void CursorMgr::set_picked(Meal meal)
{
	meal_picked = meal;
}
Meal CursorMgr::get_picked()
{
	return meal_picked;
}