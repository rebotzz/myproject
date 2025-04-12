#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "resources_manager.h"

#include <string>

inline void render_text(SDL_Renderer* renderer, const std::string& text, const SDL_Rect& rect, const SDL_Color& color, const std::string& font = "IPix")
{
	SDL_Surface* suf_text = TTF_RenderUTF8_Solid(ResMgr::instance()->find_font(font), text.c_str(), color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
	SDL_Rect rect_text = rect;
	SDL_QueryTexture(tex_text, nullptr, nullptr, &rect_text.w, &rect_text.h);
	if (rect_text.w > rect.w || rect_text.h > rect.h)		// 文字超过给定矩形时，缩放
	{
		float scale = std::min(rect.w / rect_text.w, rect.h / rect_text.h);
		rect_text.w *= scale;
		rect_text.h *= scale;
	}

	SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
	SDL_FreeSurface(suf_text);
	SDL_DestroyTexture(tex_text);
}

// 实现格式化渲染，不同未知不同颜色
inline void render_textEx()
{
	// todo
}

inline void render_text(SDL_Renderer* renderer, const std::string& text, const SDL_Point& center_point, const SDL_Color& color, const std::string& font = "IPix")
{
	SDL_Surface* suf_text = TTF_RenderUTF8_Solid(ResMgr::instance()->find_font(font), text.c_str(), color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
	SDL_Rect rect_text = { 0 };
	SDL_QueryTexture(tex_text, nullptr, nullptr, &rect_text.w, &rect_text.h);
	rect_text.x = center_point.x - rect_text.w / 2;
	rect_text.y = center_point.y - rect_text.h / 2;

	SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
	SDL_FreeSurface(suf_text);
	SDL_DestroyTexture(tex_text);
}