#pragma once
#include "region.h"
#include "meal.h"

class Bundle : public Region
{
private:
	SDL_Texture* _texture = nullptr;
	Meal _meal = Meal::None;

public:
	Bundle(int x, int y, int w, int h, SDL_Texture* texture, Meal meal) 
		:Region({ x, y, w, h }), _texture(texture), _meal(meal) {};
	~Bundle() = default;

	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_down() override;
	virtual void on_cursor_up() override;
};
