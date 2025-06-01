#pragma once

#include "config_manager.h"
#include "resources_manager.h"
#include "tower_manager.h"
#include "home_manager.h"
#include "coin_manager.h"

#include "SDL2_gfxPrimitives.h"
#include <cassert>
#include <string>


// UI����(����)�����÷�����/����������
class Panel
{
	// ѡ�и�������
	enum class GridType
	{
		None = 0,				// ���ܲ���
		Place,					// ���Է�������
		Upgrade					// �������� home
	};

	// ����ִ�в�������
	enum class SelectType
	{
		None = 0,				// ��Ч��
		TopAxeman,				// ������
		LeftArcher,				// ��ͷ��
		RightGunner				// ����
	};


public:
	Panel() = default;
	~Panel() = default;

	void on_input(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			// 2.��ѡ��ִ�в���
			pos_cursor.x = event.motion.x;
			pos_cursor.y = event.motion.y;
			if (is_selected_grid) check_select_type();
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button != SDL_BUTTON_LEFT) break;
			// 1.��ѡ������
			pos_grid.x = event.motion.x / TILE_SIZE;
			pos_grid.y = event.motion.y / TILE_SIZE;
			if (check_can_select()) is_selected_grid = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button != SDL_BUTTON_LEFT) break;
			// 3.ִ�в���
			if (is_selected_grid)
			{
				is_selected_grid = false;
				if (SelectType::None == select_type) break;

				// ��������/��������
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
		// todo: ����������̲���ʱʱ����٣�
	}

	void on_render(SDL_Renderer* renderer)
	{
		if (!is_selected_grid) return;

		// ����ѡ��ui
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

		// ���ƹ�����Χ
		if (attack_range > 0)
		{
			filledCircleColor(renderer, pos_grid_center.x, pos_grid_center.y, attack_range, 0x80E3E27D);	// ʵ��Բ
			aacircleColor(renderer, pos_grid_center.x, pos_grid_center.y, attack_range, 0xFFFFFFFF);		// ��ɫ����ݱ߿�
		}

		// ��������
		SDL_RenderCopy(renderer, tex_panel, nullptr, &rect_panel);

		// ���ƻ���
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
			// ������û����/û�е�ͼװ��/���ǵ����н�·��/���ǵ���ˢ�µ�
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

		// �����ж�
		Vector2 pos_grid_center = { (float)(pos_grid.x * TILE_SIZE + TILE_SIZE / 2), (float)(pos_grid.y * TILE_SIZE + TILE_SIZE / 2) };
		int angle = ((int)(pos_cursor - pos_grid_center).angle() + 360) % 360;
		float lenth = (pos_cursor - pos_grid_center).lenth();
		if (lenth > TILE_SIZE / 2 && lenth < TILE_SIZE * 1.7)
		{
			if (angle > 240 && angle < 330) select_type = SelectType::TopAxeman;
			else if(angle > 90 && angle < 240) select_type = SelectType::LeftArcher;
			else select_type = SelectType::RightGunner;
		}

		// �ж����軨��
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
	Vector2 pos_cursor;								// ���λ��(��������)
	SDL_Point pos_grid;								// ѡ�и�������(�������꣬��λ����)
	bool is_selected_grid = false;					// �Ƿ��Ѿ�ѡ��ø���
	GridType grid_type = GridType::None;			// ѡ�и�������
	SelectType select_type = SelectType::None;		// ִ�е�����ѡ��
	int cost = -1;									// �����������
};
