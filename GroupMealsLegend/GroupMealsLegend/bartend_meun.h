#pragma once
#include "button.h"
#include "meal.h"
#include <array>
#include <vector>

// ���Ʋ˵�		

// ��һ�£�ֻҪ��ʼ������䷽���棬��Ҫ����
class BartendMeun : public Region
{
private:
	Meal drink = Meal::None;
	int idx = 0;											// �䷽ҳ��
	Button button_prev;
	Button button_next;
	std::array<std::vector<std::string>, 4> formula_list;	// �䷽
	SDL_Rect rect_btc_icon;

	// ��ȾЧ�����
	std::array<std::vector<std::vector<std::string>>, 4> formula_list_seg;
	std::array<std::vector<std::vector<SDL_Color>>, 4> formula_list_color;

public:
	BartendMeun();
	~BartendMeun();

	void on_render(SDL_Renderer* renderer) override;
	//void on_cursor_up() override;

	void set_valid(bool flag);	// ���ǻ���
	void reset() { idx = 0; }

private:
	void render_formula(SDL_Renderer* renderer);
	void init_render_format();
};