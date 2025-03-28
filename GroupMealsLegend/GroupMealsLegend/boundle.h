#pragma once
#include "region.h"
#include "meal.h"

class Boundle : public Region
{
private:
	SDL_Texture* _texture = nullptr;
	Meal _meal = Meal::None;

public:
	Boundle(int x, int y, int w, int h, SDL_Texture* texture, Meal meal) 
		:Region({ x, y, w, h }), _texture(texture), _meal(meal) {};
	~Boundle() = default;

	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_down() override;
	virtual void on_cursor_up() override;
};
