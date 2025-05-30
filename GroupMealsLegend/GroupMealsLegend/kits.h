#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "resources_manager.h"

#include <string>

inline SDL_Rect render_text(SDL_Renderer* renderer, const std::string& text, const SDL_Rect& rect, const SDL_Color& color, const std::string& font = "simhei")
{
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(ResMgr::instance()->find_font(font), text.c_str(), color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
	SDL_Rect rect_text = rect;
	SDL_QueryTexture(tex_text, nullptr, nullptr, &rect_text.w, &rect_text.h);
	if (rect_text.w > rect.w || rect_text.h > rect.h)		// 文字超过给定矩形时，缩放
	{
		float scale = std::min((float)rect.w / (float)rect_text.w, (float)rect.h / (float)rect_text.h);	// debug: (float)
		rect_text.w *= scale;
		rect_text.h *= scale;
	}

	SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
	SDL_FreeSurface(suf_text);
	SDL_DestroyTexture(tex_text);
	return rect_text;
}

// 实现格式化渲染，不同颜色
inline SDL_Point render_textEx(SDL_Renderer* renderer, const std::vector<std::string>& texts, const std::vector<SDL_Color>& colors,
	const SDL_Point& left_top_point, const std::string& font = "simhei", float scale = 1.0)
{
	if (texts.size() != colors.size())
		return left_top_point;

	SDL_Point new_line_point = left_top_point;
	for (int i = 0; i < texts.size(); ++i)
	{
		SDL_Surface* suf_text = TTF_RenderUTF8_Solid(ResMgr::instance()->find_font(font), texts[i].c_str(), colors[i]);
		SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
		SDL_Rect rect_text = { new_line_point.x, new_line_point.y, 0, 0 };
		SDL_QueryTexture(tex_text, nullptr, nullptr, &rect_text.w, &rect_text.h);
		rect_text.w *= scale;
		rect_text.h *= scale;
		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
		SDL_FreeSurface(suf_text);
		SDL_DestroyTexture(tex_text);

		new_line_point.x += rect_text.w;
	}

	return new_line_point;
}

inline void render_text(SDL_Renderer* renderer, const std::string& text, const SDL_Point& center_point,
	const SDL_Color& color = { 255,255,255,255 }, const std::string& font = "simhei", float scale = 1.0)
{
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(ResMgr::instance()->find_font(font), text.c_str(), color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
	SDL_Rect rect_text = { 0 };
	SDL_QueryTexture(tex_text, nullptr, nullptr, &rect_text.w, &rect_text.h);
	rect_text.w *= scale;
	rect_text.h *= scale;
	rect_text.x = center_point.x - rect_text.w / 2;
	rect_text.y = center_point.y - rect_text.h / 2;

	SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
	SDL_FreeSurface(suf_text);
	SDL_DestroyTexture(tex_text);
}



inline void render_text_lines(SDL_Renderer* renderer, const std::vector<std::string>& lines, const SDL_Point& left_top_point,
	float scale = 1.0, const SDL_Color& color = { 255,255,255,255 }, const std::string& font = "simhei")
{
	SDL_Point point = left_top_point;
	for (int i = 0; i < lines.size(); ++i)
	{
		SDL_Surface* suf_text = TTF_RenderUTF8_Solid(ResMgr::instance()->find_font(font), lines[i].c_str(), color);
		SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
		SDL_Rect rect_text = { point.x, point.y, 0, 0 };
		SDL_QueryTexture(tex_text, nullptr, nullptr, &rect_text.w, &rect_text.h);
		rect_text.w *= scale;
		rect_text.h *= scale;
		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
		SDL_FreeSurface(suf_text);
		SDL_DestroyTexture(tex_text);

		point.y += rect_text.h;
	}
}

//inline void render_text_linesEx(SDL_Renderer* renderer, const SDL_Point& left_top_point, const std::vector<std::vector<std::string>>& lines,
//	const std::vector<std::vector<SDL_Color>>& format_color, float scale = 1.0, const std::string& font = "simhei")
//{
//	SDL_Point point = left_top_point;
//	for (int i = 0; i < lines.size(); ++i)
//	{
//		auto [x, y] = render_textEx(renderer, lines[i], format_color[i], point, font, scale);
//		point.y += y;
//	}
//}

inline void render_img(SDL_Renderer* renderer, SDL_Texture* tex, const SDL_Point& center_point, double angle = 0.0)
{
	SDL_Rect rect_img{ center_point.x, center_point.y, 0, 0 };
	SDL_QueryTexture(tex, nullptr, nullptr, &rect_img.w, &rect_img.h);
	rect_img.x = center_point.x - rect_img.w / 2;
	rect_img.y = center_point.y - rect_img.h / 2;
	SDL_Point rotate_point = { rect_img.w / 2, rect_img.h / 2 };
	SDL_RenderCopyEx(renderer, tex, nullptr, &rect_img, angle, &rotate_point, SDL_FLIP_NONE);
}

