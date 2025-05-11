#pragma once
#include "SDL.h"
#include "meal.h"
#include <string>

class CursorMgr
{
private:
	static CursorMgr* manager;
	
	Meal meal_picked = Meal::None;		// 当前抓取餐品
	SDL_Point pos_cursor = { 0, 0 };	// 光标位置
	bool is_mouse_lbtn_down = false;	// 鼠标左键是否按下
	int coins = 0;						// 获取硬币
	int coins_goal = 100;				// 目标赚取硬币数目
	bool is_bartending = false;			// 是否是酒保模式
	Meal target_drink = Meal::None;		// 目标所需要的酒
	std::string target_drink_name;		// 目标所需要的酒名字

private:
	CursorMgr() = default;
	~CursorMgr() = default;

public:
	static CursorMgr* instance();
	void on_input(const SDL_Event& event);
	void on_render(SDL_Renderer* renderer);
	void set_picked(Meal meal);				// 光标管理器本身不负责抓取，当光标移动到对应的槽位时，槽位设置抓取内容
	Meal get_picked();
	void add_coins(int val, bool play_audio = true);

	const SDL_Point& get_position() const;

	void set_coins_goal(int val);
	void set_drink_goal(const std::string& target);
	Meal get_drink_goal() const;
	const std::string& get_drink_goal_name() const;
	bool is_button_down() const;
	void enable_bartend(bool flag);			// 是否开启酒保模式
	int get_coins() const { return coins; }	// 存档相关
};