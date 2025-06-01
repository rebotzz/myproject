#pragma once

#include "config_manager.h"
#include "resources_manager.h"
#include "tower_manager.h"
#include "home_manager.h"
#include "coin_manager.h"

#include "SDL2_gfxPrimitives.h"
#include <cassert>
#include <string>


// UI界面(轮盘)：放置防御塔/升级防御塔
class Panel
{
	// 选中格子类型
	enum class GridType
	{
		None = 0,				// 不能操作
		Place,					// 可以放置塔防
		Upgrade					// 升级塔防 home
	};

	// 轮盘执行操作类型
	enum class SelectType
	{
		None = 0,				// 无效果
		TopAxeman,				// 弓箭手
		LeftArcher,				// 斧头兵
		RightGunner				// 炮手
	};


public:
	Panel() = default;
	~Panel() = default;

	void on_input(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			// 2.再选择执行操作
			pos_cursor.x = event.motion.x;
			pos_cursor.y = event.motion.y;
			if (is_selected_grid) check_select_type();
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button != SDL_BUTTON_LEFT) break;
			// 1.先选定格子
			pos_grid.x = event.motion.x / TILE_SIZE;
			pos_grid.y = event.motion.y / TILE_SIZE;
			if (check_can_select()) is_selected_grid = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button != SDL_BUTTON_LEFT) break;
			// 3.执行操作
			if (is_selected_grid)
			{
				is_selected_grid = false;
				if (SelectType::None == select_type) break;

				// 放置类型/升级类型
				TowerType tower_type = TowerType::Archer;
				switch (select_type)
				{
				case Panel::SelectType::TopAxeman:
					tower_type = TowerType::Axeman;
					break;
				case Panel::SelectType::LeftArcher:
					tower_type = TowerType::Archer;
					break;
				case Panel::SelectType::RightGunner:
					tower_type = TowerType::Gunner;
					break;
				}

				if (cost < 0 || cost >  CoinMgr::instance()->get_coins_num()) break;
				if (GridType::Place == grid_type)
				{
					TowerMgr::instance()->create_tower(tower_type,
						{ (float)(pos_grid.x * TILE_SIZE + TILE_SIZE / 2), (float)(pos_grid.y * TILE_SIZE + TILE_SIZE / 2) });
					assert(pos_grid.y >= 0 && pos_grid.y < ConfigMgr::instance()->tile_map.size()
						&& pos_grid.x >= 0 && pos_grid.x < ConfigMgr::instance()->tile_map[0].size());
					ConfigMgr::instance()->tile_map[pos_grid.y][pos_grid.x].has_tower = true;
				}
				else if (GridType::Upgrade == grid_type)
				{
					HomeMgr::instance()->upgrade_tower(tower_type);
				}
				CoinMgr::instance()->decrease_coins(cost);
			}
			break;
		}
	}

	void on_update(float delta)
	{
		// todo: 或许可以轮盘操作时时间减速，
	}

	void on_render(SDL_Renderer* renderer)
	{
		if (!is_selected_grid) return;

		// 绘制选中ui
		SDL_Point pos_grid_center = { pos_grid.x * TILE_SIZE + TILE_SIZE / 2, pos_grid.y * TILE_SIZE + TILE_SIZE / 2 };
		static SDL_Texture* tex_ui_select_cursor = ResMgr::instance()->find_texture(ResID::Tex_UiSelectCursor);
		SDL_Rect rect_cursor = { pos_grid_center.x - TILE_SIZE / 2, pos_grid_center.y - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE };
		SDL_RenderCopy(renderer, tex_ui_select_cursor, nullptr, &rect_cursor);

		SDL_Rect rect_panel = { pos_grid_center.x - 144 / 2, pos_grid_center.y - 144 / 2, 144, 144 };
		SDL_Texture* tex_panel = nullptr;
		float attack_range = -1;
		switch (select_type)
		{
		case Panel::SelectType::None:
			tex_panel = ResMgr::instance()->find_texture(grid_type == GridType::Place ? ResID::Tex_UiPlaceIdle : ResID::Tex_UiUpgradeIdle);
			break;
		case Panel::SelectType::TopAxeman:
			tex_panel = ResMgr::instance()->find_texture(grid_type == GridType::Place ? ResID::Tex_UiPlaceHoveredTop : ResID::Tex_UiUpgradeHoveredTop);
			attack_range = ConfigMgr::instance()->tower_axeman_template.view_range[HomeMgr::instance()->get_tower_level(TowerType::Axeman)] * TILE_SIZE;
			break;
		case Panel::SelectType::LeftArcher:
			tex_panel = ResMgr::instance()->find_texture(grid_type == GridType::Place ? ResID::Tex_UiPlaceHoveredLeft : ResID::Tex_UiUpgradeHoveredLeft);
			attack_range = ConfigMgr::instance()->tower_archer_template.view_range[HomeMgr::instance()->get_tower_level(TowerType::Archer)] * TILE_SIZE;
			break;
		case Panel::SelectType::RightGunner:
			tex_panel = ResMgr::instance()->find_texture(grid_type == GridType::Place ? ResID::Tex_UiPlaceHoveredRight : ResID::Tex_UiUpgradeHoveredRight);
			attack_range = ConfigMgr::instance()->tower_gunner_template.view_range[HomeMgr::instance()->get_tower_level(TowerType::Gunner)] * TILE_SIZE;
			break;
		}

		// 绘制攻击范围
		if (attack_range > 0)
		{
			filledCircleColor(renderer, pos_grid_center.x, pos_grid_center.y, attack_range, 0x80E3E27D);	// 实心圆
			aacircleColor(renderer, pos_grid_center.x, pos_grid_center.y, attack_range, 0xFFFFFFFF);		// 白色抗锯齿边框
		}

		// 绘制轮盘
		SDL_RenderCopy(renderer, tex_panel, nullptr, &rect_panel);

		// 绘制花费
		if (SelectType::None == select_type) return;
		std::string text = cost > 0 ? std::to_string(cost) : "Max";
		SDL_Surface* suf_text = TTF_RenderUTF8_Blended(ResMgr::instance()->find_font(ResID::Font_Ipix), text.c_str(), { 255,255,255,255 });
		SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
		SDL_Rect rect_text = {
		(int)(pos_grid_center.x - suf_text->w / 2),
		(int)(pos_grid_center.y - suf_text->h / 2 + TILE_SIZE * 1.3),
		suf_text->w , suf_text->h
		};
		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
		SDL_FreeSurface(suf_text);
		SDL_DestroyTexture(tex_text);
	}

private:
	bool check_can_select()
	{
		const auto& tile_map = ConfigMgr::instance()->tile_map;
		const auto& pos_home = ConfigMgr::instance()->home;
		if (pos_grid.x == pos_home.x && pos_grid.y == pos_home.y)
		{
			grid_type = GridType::Upgrade;
			return true;
		}

		if (pos_grid.y >= 0 && pos_grid.y < tile_map.size()
			&& pos_grid.x >= 0 && pos_grid.x < tile_map[0].size())
		{
			auto& tile = tile_map[pos_grid.y][pos_grid.x];
			// 条件：没有塔/没有地图装饰/不是敌人行进路线/不是敌人刷新点
			if (!tile.has_tower && tile.direction == Tile::Direction::None && tile.decoration == -1 && tile.speacial_flag < 0)
			{
				grid_type = GridType::Place;
				return true;
			}
		}

		grid_type = GridType::None;
		return false;
	}

	SelectType check_select_type()
	{
		if(!is_selected_grid) return SelectType::None;
		select_type = SelectType::None;

		// 区域判断
		Vector2 pos_grid_center = { (float)(pos_grid.x * TILE_SIZE + TILE_SIZE / 2), (float)(pos_grid.y * TILE_SIZE + TILE_SIZE / 2) };
		int angle = ((int)(pos_cursor - pos_grid_center).angle() + 360) % 360;
		float lenth = (pos_cursor - pos_grid_center).lenth();
		if (lenth > TILE_SIZE / 2 && lenth < TILE_SIZE * 1.7)
		{
			if (angle > 240 && angle < 330) select_type = SelectType::TopAxeman;
			else if(angle > 90 && angle < 240) select_type = SelectType::LeftArcher;
			else select_type = SelectType::RightGunner;
		}

		// 判断所需花费
		static HomeMgr* home = HomeMgr::instance();
		switch (select_type)
		{
		case Panel::SelectType::TopAxeman:
			cost = (grid_type == GridType::Place ? home->get_tower_place_cost(TowerType::Axeman) : home->get_tower_upgrade_cost(TowerType::Axeman));
			break;
		case Panel::SelectType::LeftArcher:
			cost = (grid_type == GridType::Place ? home->get_tower_place_cost(TowerType::Archer) : home->get_tower_upgrade_cost(TowerType::Archer));
			break;
		case Panel::SelectType::RightGunner:
			cost = (grid_type == GridType::Place ? home->get_tower_place_cost(TowerType::Gunner) : home->get_tower_upgrade_cost(TowerType::Gunner));
			break;
		}

		return select_type;
	}


private:
	Vector2 pos_cursor;								// 鼠标位置(世界坐标)
	SDL_Point pos_grid;								// 选中格子坐标(格子坐标，单位格子)
	bool is_selected_grid = false;					// 是否已经选择好格子
	GridType grid_type = GridType::None;			// 选中格子类型
	SelectType select_type = SelectType::None;		// 执行的轮盘选项
	int cost = -1;									// 操作所需费用
};
