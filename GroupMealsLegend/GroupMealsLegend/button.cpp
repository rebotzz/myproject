#include "button.h"
#include "kits.h"
#include "cursor_manager.h"

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
void Button::set_texture(SDL_Texture* tex)
{
	texture = tex;
}
void Button::set_enable(bool flag)
{
	is_enabled = flag;
}
void Button::set_position(int x, int y)
{
	rect.x = x, rect.y = y;
}


void Button::on_render(SDL_Renderer* renderer)
{
	if (!is_enabled)
	{
		SDL_RenderCopy(renderer, ResMgr::instance()->find_texture("button_1"), nullptr, &rect);
		SDL_Point point = { rect.x + rect.w / 2, rect.y + rect.h / 2 };
		render_text(renderer, desc, point, { 0,0,0,255 });
	}
	else
	{
		SDL_Rect rect_render = rect;
		if (is_button_down)
		{
			rect_render.x += 3;
			rect_render.y += 3;
		}
		SDL_RenderCopy(renderer, texture, nullptr, &rect_render);
		SDL_Point point = { rect_render.x + rect_render.w / 2, rect_render.y + rect_render.h / 2 };
		render_text(renderer, desc, point, { 0,0,0,255 });
	}
}
void Button::on_cursor_up()
{
	if (!is_enabled) return;

	is_button_down = false;
	if (on_click)
		on_click();
}
void Button::on_cursor_down()
{
	if (!is_enabled) return;

	is_button_down = true;
}

void Button::on_update(float delta)
{
	if (is_button_down && !CursorMgr::instance()->is_button_down())
		is_button_down = false;
}

void ButtonText::on_render(SDL_Renderer* renderer)
{
	SDL_Color text_color = color;
	if (!SDL_PointInRect(&CursorMgr::instance()->get_position(), &rect))
	{
		text_color.a *= 0.6;
	}
	Button::on_render(renderer);
}
