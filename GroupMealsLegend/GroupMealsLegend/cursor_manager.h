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

private:
	CursorMgr() = default;
	~CursorMgr() = default;

public:
	static CursorMgr* instance();
	void input(const SDL_Event& event);
	void render(SDL_Renderer* renderer);
	void set_picked(Meal meal);				// 光标管理器本身不负责抓取，当光标移动到对应的槽位时，槽位设置抓取内容
	Meal get_picked();
};