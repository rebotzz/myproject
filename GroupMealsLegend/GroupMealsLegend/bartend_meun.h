#pragma once
#include "button.h"
#include "meal.h"
#include <array>

// ���Ʋ˵�		

// ��һ�£�ֻҪ��ʼ������䷽���棬��Ҫ����
class BartendMeun
{
private:
	enum class Status
	{
		Init,									// ��ʼ��ӭ����
		Describe								// �����䷽����
	};
private:
	Meal drink = Meal::None;
	int idx = 0;								// �䷽ҳ��
	bool valid = true;							// �Ƿ�����
	Status status = Status::Init;				// ��ʼ״̬
	SDL_Rect rect_meun = { 0, 0, 662, 486 };	// �˵����ھ���
	Button button_prev;
	Button button_next;
	std::array<std::string, 4> formula_list;		// �䷽

public:
	BartendMeun();
	~BartendMeun();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void set_valid(bool flag);
	void reset() { status = Status::Init; }
};