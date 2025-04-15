#pragma once
#include "button.h"
#include "meal.h"
#include <array>

// 调酒菜单		

// 简化一下，只要初始界面和配方界面，不要索引
class BartendMeun
{
private:
	enum class Status
	{
		Init,									// 初始欢迎界面
		Describe								// 具体配方界面
	};
private:
	Meal drink = Meal::None;
	int idx = 0;								// 配方页号
	bool valid = true;							// 是否启用
	Status status = Status::Init;				// 初始状态
	SDL_Rect rect_meun = { 0, 0, 662, 486 };	// 菜单所在矩形
	Button button_prev;
	Button button_next;
	std::array<std::string, 4> formula_list;		// 配方

public:
	BartendMeun();
	~BartendMeun();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void set_valid(bool flag);
	void reset() { status = Status::Init; }
};