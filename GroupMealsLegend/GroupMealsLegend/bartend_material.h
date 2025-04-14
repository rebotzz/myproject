#pragma once
#include "region.h"
#include "meal.h"

// 抽象调酒原料
class RawMaterial : public Region
{
protected:
	int count = 0;							// 原料加入份量
	SDL_Color color;						// 绘制颜色
	Meal material = Meal::None;				// 原料类型

public:
	RawMaterial(Meal _meal, SDL_Rect _rect, SDL_Color _color = { 255,255,255,255 });

	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_down() override;
	virtual void on_cursor_up() override;
	void set_count(int val);
};

// 具体调酒原料
class Adelhyde : public RawMaterial
{
public:
	Adelhyde() :RawMaterial(Meal::Adelhyde, { 810,330, 145, 115 }, { 225, 40, 65, 255 }) {}
};

class BronsonExt : public RawMaterial
{
public:
	BronsonExt() :RawMaterial(Meal::BronsonExt, { 960,330, 145, 115 }, { 255, 200, 60, 255 }) {}
};

class PwdDelta : public RawMaterial
{
public:
	PwdDelta() :RawMaterial(Meal::PwdDelta, { 1110,330, 145, 115 }, { 135, 165, 225, 255 }) {}
};

class Flanergide : public RawMaterial
{
public:
	Flanergide() :RawMaterial(Meal::Flanergide, { 810,450, 145, 115 }, { 160, 200, 115, 255 }) {}
};

class Karmotrine : public RawMaterial
{
public:
	Karmotrine() :RawMaterial(Meal::Karmotrine, { 1110,450, 145, 115 }, { 200, 230, 230, 255 }) {}
};

// 加冰
class Ice : public RawMaterial
{
public:
	Ice() :RawMaterial(Meal::Ice, { 740, 370, 50,50 }) {}
	void on_render(SDL_Renderer* renderer) override;
};

// 陈化
class Ageing : public RawMaterial
{
public:
	Ageing() :RawMaterial(Meal::Ageing, { 740, 500, 50,50 }) {}
	void on_render(SDL_Renderer* renderer) override;
};