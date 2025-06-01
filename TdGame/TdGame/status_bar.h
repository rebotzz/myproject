#pragma once

#include "resources_manager.h"
#include "home_manager.h"
#include "coin_manager.h"
#include "player_dragon_manager.h"

#include "SDL2_gfxPrimitives.h"

// UI界面(状态栏)
class StatusBar
{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void on_render(SDL_Renderer* renderer)
	{
		// home icon/hp
		static SDL_Rect rect_home = { 20, 20, 78,78 };
		static SDL_Texture* tex_home = ResMgr::instance()->find_texture(ResID::Tex_UiHomeAvatar);
		SDL_RenderCopy(renderer, tex_home, nullptr, &rect_home);
		int hp = HomeMgr::instance()->get_hp();
		static SDL_Texture* tex_heart = ResMgr::instance()->find_texture(ResID::Tex_UiHeart);
		SDL_Rect rect_heart = { rect_home.x + rect_home.w + 10, rect_home.y, 32, 32 };
		for (int i = 0; i < hp; ++i)
		{
			SDL_RenderCopy(renderer, tex_heart, nullptr, &rect_heart);
			rect_heart.x += rect_heart.w + 5;
		}

		// coin icon/count
		static SDL_Texture* tex_coin = ResMgr::instance()->find_texture(ResID::Tex_UiCoin);
		static SDL_Rect rect_coin = { rect_home.x + rect_home.w + 10, rect_home.y + rect_home.h - 32, 32, 32 };
		SDL_RenderCopy(renderer, tex_coin, nullptr, &rect_coin);

		SDL_Surface* suf_coin_num = TTF_RenderUTF8_Blended(ResMgr::instance()->find_font(ResID::Font_Ipix), 
			std::to_string(CoinMgr::instance()->get_coins_num()).c_str(), { 255,255,255,255 });
		SDL_Texture* tex_coin_num = SDL_CreateTextureFromSurface(renderer, suf_coin_num);
		SDL_Rect rect_coin_num = { rect_coin.x + rect_coin.w + 10, rect_coin.y + rect_coin.h / 2 - suf_coin_num->h / 2, suf_coin_num->w , suf_coin_num->h };
		SDL_RenderCopy(renderer, tex_coin_num, nullptr, &rect_coin_num);
		SDL_FreeSurface(suf_coin_num);
		SDL_DestroyTexture(tex_coin_num);

		// 玩家闪电龙/mp
		static SDL_Texture* tex_player = ResMgr::instance()->find_texture(ResID::Tex_UiPlayerAvatar);
		static SDL_Rect rect_player = { rect_home.x + rect_home.w / 2 - 65 / 2, rect_home.y + rect_home.h + 15, 65, 65 };
		static SDL_Rect rect_mp = { rect_player.x + rect_player.w + 10, rect_player.y + rect_player.h / 2 - 20 / 2 , 150, 20 };
		SDL_RenderCopy(renderer, tex_player, nullptr, &rect_player);
		roundedBoxRGBA(renderer, rect_mp.x, rect_mp.y, rect_mp.x + rect_mp.w, rect_mp.y + rect_mp.h, 4, 120, 120, 120, 255);
		roundedBoxRGBA(renderer, rect_mp.x + 2, rect_mp.y + 2, rect_mp.x + rect_mp.w * PlayerDragonMgr::instance()->get_mp_percentage() - 4,
			rect_mp.y + rect_mp.h - 4, 4, 140, 60, 190, 255);

	}


};


