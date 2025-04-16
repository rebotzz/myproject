#pragma once
#include "button.h"
#include "meal.h"
#include <array>
#include <vector>

// 调酒菜单		

// 简化一下，只要初始界面和配方界面，不要索引
class BartendMeun : public Region
{
private:
	Meal drink = Meal::None;
	int idx = 0;											// 配方页号
	Button button_prev;
	Button button_next;
	std::array<std::vector<std::string>, 4> formula_list;	// 配方
	SDL_Rect rect_btc_icon;

	// 渲染效果相关
	std::array<std::vector<std::vector<std::string>>, 4> formula_list_seg;
	std::array<std::vector<std::vector<SDL_Color>>, 4> formula_list_color;

public:
	BartendMeun();
	~BartendMeun();

	void on_render(SDL_Renderer* renderer) override;
	//void on_cursor_up() override;

	void set_valid(bool flag);	// 覆盖基类
	void reset() { idx = 0; }

private:
	void render_formula(SDL_Renderer* renderer);
	void init_render_format();
};