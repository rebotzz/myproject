#include "bartend_material.h"
#include "cursor_manager.h"
#include "resources_manager.h"

RawMaterial::RawMaterial(Meal _meal, SDL_Rect _rect, SDL_Color _color)
	:Region(_rect), material(_meal), color(_color)
{
}

void RawMaterial::set_count(int val)
{
	count = val;
}

void RawMaterial::on_render(SDL_Renderer* renderer)
{
	// 绘制表示原料加入份量的矩形
	SDL_Rect base_rect = { rect.x + 50, rect.y + 30, 90 / 5 - 5, 70 / 2 - 5};

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a * 0.5);
	for (int i = 0; i < 10; ++i)
	{
		SDL_Rect count_rect = base_rect;
		count_rect.x += (i % 5) * (base_rect.w + 2) + 4;
		count_rect.y += ((i / 5) % 2) * (base_rect.h + 2) + 4;

		SDL_RenderDrawRect(renderer, &count_rect);
	}
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int i = 0; i < count; ++i)
	{
		SDL_Rect count_rect = base_rect;
		count_rect.x += (i % 5) * base_rect.w + 4;
		count_rect.y += ((i / 5) % 2) * base_rect.h + 4;

		if (i >= 10) SDL_RenderFillRect(renderer, &count_rect);
		else SDL_RenderDrawRect(renderer, &count_rect);
	}
}
void RawMaterial::on_cursor_down()
{
	if (CursorMgr::instance()->get_picked() == Meal::None)
	{
		CursorMgr::instance()->set_picked(material);
		count++;
	}
}
void RawMaterial::on_cursor_up()
{
	if (CursorMgr::instance()->get_picked() == material)
	{
		CursorMgr::instance()->set_picked(Meal::None);
		count--;
	}
}

void Ice::on_render(SDL_Renderer* renderer)
{
	static SDL_Texture* tex = ResMgr::instance()->find_texture("ice");
	SDL_RenderCopy(renderer, tex, nullptr, &rect);
}

void Ageing::on_render(SDL_Renderer* renderer)
{
	static SDL_Texture* tex = ResMgr::instance()->find_texture("ageing");
	SDL_RenderCopy(renderer, tex, nullptr, &rect);
}