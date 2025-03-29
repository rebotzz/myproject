#pragma once
#include "region.h"
#include "timer.h"
#include "meal.h"
#include <vector>

class DeliveryDriver : public Region
{
private:
	Timer timer_refresh;
	std::vector<std::pair<Meal, bool>> list;	// k-v: 餐品，是否需要（餐品状态:收到-未收到）
	bool is_meituan = true;
	bool is_waiting = false;
	float waited_time = 0;
	float waited_time_limit = 40.0f;

public:
	DeliveryDriver(int x, int y);
	~DeliveryDriver() = default;

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;

	void refresh();
};