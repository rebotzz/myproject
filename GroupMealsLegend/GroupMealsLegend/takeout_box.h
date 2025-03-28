#pragma once
#include "region.h"
#include "meal.h"
#include <unordered_set>

class TakeoutBox : public Region
{
private:
	Meal meal = Meal::None;
	std::unordered_set<Meal> can_place_list;

public:
	TakeoutBox(int x, int y);
	~TakeoutBox() = default;

	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_down() override;
	virtual void on_cursor_up() override;

private:
	bool can_place(Meal id);
};