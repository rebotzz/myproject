#include "button.h"
#include "kits.h"

Button::Button(int x, int y) :Region({x, y, 134, 46})
{
	texture = ResMgr::instance()->find_texture("button_1");
}

void Button::set_describe(const std::string& str)
{
	desc = str;
}
void Button::set_on_click(const std::function<void(void)>& callback)
{
	on_click = callback;
}

void Button::on_render(SDL_Renderer* renderer)
{
	SDL_Rect rect_render = rect;
	if (is_button_down)
	{
		rect_render.x += 3;
		rect_render.y += 3;
	}
	SDL_RenderCopy(renderer, texture, nullptr, &rect_render);
	SDL_Point point = { rect_render.x + rect_render.w / 2, rect_render.y + rect_render.h / 2};
	render_text(renderer, desc, point, { 0,0,0,255 });
}
void Button::on_cursor_up()
{
	is_button_down = false;
	if (on_click)
		on_click();
}
void Button::on_cursor_down()
{
	is_button_down = true;
}


void ButtonModulate::on_render(SDL_Renderer* renderer)
{
	switch (status)
	{
	case Status::Init: set_describe(u8"调制"); break;
	case Status::Doing: set_describe(u8"停止"); break;
	case Status::Done: set_describe(u8"提交"); break;
	}
	Button::on_render(renderer);
}
