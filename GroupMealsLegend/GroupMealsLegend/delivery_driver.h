#pragma once
#include "region.h"
#include "timer.h"
#include "meal.h"
#include <vector>

class DeliveryDriver : public Region
{
private:
	Timer refresh_timer;
	std::vector<std::pair<Meal, bool>> list;	// k-v: 所需餐品，是否收到
	bool is_meituan = true;
	bool is_waiting = true;
	float pass_time = 0;
	float wait_time = 30.0f;

public:
	DeliveryDriver(int x, int y);
	~DeliveryDriver() = default;

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;
	virtual void on_cursor_down() override;

	bool can_place(Meal meal);
	void refresh();
};