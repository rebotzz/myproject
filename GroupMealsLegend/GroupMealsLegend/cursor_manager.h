#pragma once
#include "SDL.h"
#include "meal.h"

class CursorMgr
{
private:
	static CursorMgr* manager;
	
	Meal meal_picked = Meal::None;		// 当前抓取餐品
	SDL_Point pos_cursor = { 0, 0 };	// 光标位置
	bool is_mouse_lbtn_down = false;	// 鼠标左键是否按下
	int coins = 0;						// 获取硬币
	int goal = 100;						// 目标赚取硬币数目

private:
	CursorMgr() = default;
	~CursorMgr() = default;

public:
	static CursorMgr* instance();
	void on_input(const SDL_Event& event);
	void on_render(SDL_Renderer* renderer);
	void set_picked(Meal meal);				// 光标管理器本身不负责抓取，当光标移动到对应的槽位时，槽位设置抓取内容
	Meal get_picked();
	void add_coins(int val);

	void set_goal(int val);
	const SDL_Point& get_position() const;
};