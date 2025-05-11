#pragma once
#include "SDL.h"
#include "meal.h"
#include <string>

class CursorMgr
{
private:
	static CursorMgr* manager;
	
	Meal meal_picked = Meal::None;		// ��ǰץȡ��Ʒ
	SDL_Point pos_cursor = { 0, 0 };	// ���λ��
	bool is_mouse_lbtn_down = false;	// �������Ƿ���
	int coins = 0;						// ��ȡӲ��
	int coins_goal = 100;				// Ŀ��׬ȡӲ����Ŀ
	bool is_bartending = false;			// �Ƿ��ǾƱ�ģʽ
	Meal target_drink = Meal::None;		// Ŀ������Ҫ�ľ�
	std::string target_drink_name;		// Ŀ������Ҫ�ľ�����

private:
	CursorMgr() = default;
	~CursorMgr() = default;

public:
	static CursorMgr* instance();
	void on_input(const SDL_Event& event);
	void on_render(SDL_Renderer* renderer);
	void set_picked(Meal meal);				// ����������������ץȡ��������ƶ�����Ӧ�Ĳ�λʱ����λ����ץȡ����
	Meal get_picked();
	void add_coins(int val, bool play_audio = true);

	const SDL_Point& get_position() const;

	void set_coins_goal(int val);
	void set_drink_goal(const std::string& target);
	Meal get_drink_goal() const;
	const std::string& get_drink_goal_name() const;
	bool is_button_down() const;
	void enable_bartend(bool flag);			// �Ƿ����Ʊ�ģʽ
	int get_coins() const { return coins; }	// �浵���
};