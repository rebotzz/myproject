#pragma once
#include "SDL.h"
#include "meal.h"

class CursorMgr
{
private:
	static CursorMgr* manager;
	
	Meal meal_picked = Meal::None;		// ��ǰץȡ��Ʒ
	SDL_Point pos_cursor = { 0, 0 };	// ���λ��
	bool is_mouse_lbtn_down = false;	// �������Ƿ���

private:
	CursorMgr() = default;
	~CursorMgr() = default;

public:
	static CursorMgr* instance();
	void input(const SDL_Event& event);
	void render(SDL_Renderer* renderer);
	void set_picked(Meal meal);				// ����������������ץȡ��������ƶ�����Ӧ�Ĳ�λʱ����λ����ץȡ����
	Meal get_picked();
};