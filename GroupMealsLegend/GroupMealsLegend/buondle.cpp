#include "bundle.h"

#include "cursor_manager.h"


void Bundle::on_render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, _texture, nullptr, &get_rect());
}

void Bundle::on_cursor_down()
{
	if (CursorMgr::instance()->get_picked() == Meal::None)
		CursorMgr::instance()->set_picked(_meal);
}

void Bundle::on_cursor_up()
{
	if (CursorMgr::instance()->get_picked() == _meal)
		CursorMgr::instance()->set_picked(Meal::None);
}