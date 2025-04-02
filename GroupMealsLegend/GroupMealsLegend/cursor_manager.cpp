#include "cursor_manager.h"
#include "resources_manager.h"
#include "kits.h"

CursorMgr* CursorMgr::manager = nullptr;
CursorMgr* CursorMgr::instance()
{
	if (nullptr == manager)
	{
		manager = new CursorMgr;
	}
	return manager;
}
void CursorMgr::on_input(const SDL_Event& event)
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
		else if (event.button.button == SDL_BUTTON_RIGHT)
		{
			meal_picked = Meal::None;
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
void CursorMgr::on_render(SDL_Renderer* renderer)
{
	// 绘制持有的餐品纹理
	SDL_Texture* texture_picked = nullptr;
	switch (meal_picked)
	{
	case Meal::None: break;
	case Meal::Cola:				texture_picked = ResMgr::instance()->find_texture("cola"); break;
	case Meal::Sprite:				texture_picked = ResMgr::instance()->find_texture("sprite"); break;
	case Meal::BraisedChicken_Hot:	texture_picked = ResMgr::instance()->find_texture("bc_hot_picked"); break;
	case Meal::BraisedChicken_Cold:	texture_picked = ResMgr::instance()->find_texture("bc_cold_picked"); break;
	case Meal::MeatBall_Hot:		texture_picked = ResMgr::instance()->find_texture("mb_hot_picked"); break;
	case Meal::MeatBall_Cold:		texture_picked = ResMgr::instance()->find_texture("mb_cold_picked"); break;
	case Meal::RedCookedPork_Hot:	texture_picked = ResMgr::instance()->find_texture("rcp_hot_picked"); break;
	case Meal::RedCookedPork_Cold:	texture_picked = ResMgr::instance()->find_texture("rcp_cold_picked"); break;
	case Meal::BraisedChicken_Box:	texture_picked = ResMgr::instance()->find_texture("bc_box"); break;
	case Meal::MeatBall_Box:		texture_picked = ResMgr::instance()->find_texture("mb_box"); break;
	case Meal::RedCookedPork_Box:	texture_picked = ResMgr::instance()->find_texture("rcp_box"); break;
	case Meal::TakeoutBox:			texture_picked = ResMgr::instance()->find_texture("tb_picked"); break;			
	default: break;
	}
	if (texture_picked)
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

	// 绘制赚取的硬币
	static SDL_Texture* tex_coin = ResMgr::instance()->find_texture("coin");
	static SDL_Rect rect_coin = { 1100, 20, 40, 40 };
	SDL_RenderCopy(renderer, tex_coin, nullptr, &rect_coin);
	//SDL_Surface* suf_text = TTF_RenderUTF8_Solid(ResMgr::instance()->find_font("IPix"), std::to_string(coins).c_str(), { 255,255,255,255 });
	//SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
	//SDL_Rect rect_text = { 1150, 25, 0,0 };
	//SDL_QueryTexture(tex_text, nullptr, nullptr, &rect_text.w, &rect_text.h);
	//SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
	//SDL_FreeSurface(suf_text);
	//SDL_DestroyTexture(tex_text);

	render_text(renderer, std::to_string(coins), { 1150, 25, INT_MAX, INT_MAX }, { 255,255,255,255 });
}
void CursorMgr::set_picked(Meal meal)
{
	meal_picked = meal;
}
Meal CursorMgr::get_picked()
{
	return meal_picked;
}

void CursorMgr::add_coins(int val)
{
	coins += val;
	Mix_PlayChannel(-1, ResMgr::instance()->find_audio("get_coins"), 0);
}