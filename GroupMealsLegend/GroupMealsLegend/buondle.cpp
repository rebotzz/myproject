#include "boundle.h"

#include "cursor_manager.h"


void Boundle::on_render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, _texture, nullptr, &get_rect());
}

void Boundle::on_cursor_down()
{
	if (CursorMgr::instance()->get_picked() == Meal::None)
		CursorMgr::instance()->set_picked(_meal);
}

void Boundle::on_cursor_up()
{
	if (CursorMgr::instance()->get_picked() == _meal)
		CursorMgr::instance()->set_picked(Meal::None);
}