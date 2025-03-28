#pragma once
#include "region.h"
#include "timer.h"
#include "meal.h"

class MicrowaveOven : public Region
{
private:
	bool is_working = false;
	Timer timer;
	Meal meal_target = Meal::None;

public:
	MicrowaveOven(int x, int y);
	~MicrowaveOven() = default;

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_down() override;
	virtual void on_cursor_up()	override;

private:
	bool can_place(Meal meal);
};

