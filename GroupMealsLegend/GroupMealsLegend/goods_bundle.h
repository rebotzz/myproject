#pragma once
#include "bundle.h"
#include "resources_manager.h"

class ColaBundle : public Bundle
{
public:
	ColaBundle(int x, int y) :Bundle(x, y, 120, 124 , ResMgr::instance()->find_texture("cola_bundle"), Meal::Cola) {};
	~ColaBundle() = default;
};

class SpriteBundle : public Bundle
{
public:
	SpriteBundle(int x, int y) :Bundle(x, y, 120, 124, ResMgr::instance()->find_texture("sprite_bundle"), Meal::Sprite) {};
	~SpriteBundle() = default;
};

class TbBundle : public Bundle
{
public:
	TbBundle(int x, int y) :Bundle(x, y, 112, 96, ResMgr::instance()->find_texture("tb_bundle"), Meal::TakeoutBox) {};
	~TbBundle() = default;
};

class MbBoxBundle : public Bundle
{
public:
	MbBoxBundle(int x, int y) :Bundle(x, y, 160, 88, ResMgr::instance()->find_texture("mb_box_bundle"), Meal::MeatBall_Box) {};
	~MbBoxBundle() = default;

	virtual void on_render(SDL_Renderer* renderer) override
	{
		Bundle::on_render(renderer);
		static SDL_Texture* texture = ResMgr::instance()->find_texture("mb_icon");
		SDL_SetTextureAlphaMod(texture, 255);
		static SDL_Rect rect_src = { 0, 0, (int)(63 * 0.7), (int)(27 * 0.7) };
		static SDL_Rect rect_dst = { _rect.x + 30, _rect.y - 5, rect_src.w, rect_src.h };
		SDL_RenderCopy(renderer, texture, nullptr, &rect_dst);
	}
};

class BcBoxBundle : public Bundle
{
public:
	BcBoxBundle(int x, int y) :Bundle(x, y, 160, 88, ResMgr::instance()->find_texture("bc_box_bundle"), Meal::BraisedChicken_Box) {};
	~BcBoxBundle() = default;

	virtual void on_render(SDL_Renderer* renderer) override
	{
		Bundle::on_render(renderer);
		static SDL_Texture* texture = ResMgr::instance()->find_texture("bc_icon");
		SDL_SetTextureAlphaMod(texture, 255);
		static SDL_Rect rect_src = { 0, 0, (int)(63 * 0.7), (int)(27 * 0.7) };
		static SDL_Rect rect_dst = { _rect.x + 30, _rect.y - 5, rect_src.w, rect_src.h };
		SDL_RenderCopy(renderer, texture, nullptr, &rect_dst);
	}
};

class RcpBoxBundle : public Bundle
{
public:
	RcpBoxBundle(int x, int y) :Bundle(x, y, 160, 88, ResMgr::instance()->find_texture("rcp_box_bundle"), Meal::RedCookedPork_Box) {};
	~RcpBoxBundle() = default;

	virtual void on_render(SDL_Renderer* renderer) override
	{
		Bundle::on_render(renderer);
		static SDL_Texture* texture = ResMgr::instance()->find_texture("rcp_icon");
		SDL_SetTextureAlphaMod(texture, 255);
		static SDL_Rect rect_src = { 0, 0, (int)(63 * 0.7), (int)(27 * 0.7) };
		static SDL_Rect rect_dst = { _rect.x + 30, _rect.y - 5, rect_src.w, rect_src.h };
		SDL_RenderCopy(renderer, texture, nullptr, &rect_dst);
	}
};

