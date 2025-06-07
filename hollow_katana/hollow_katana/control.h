#pragma once
#include "character.h"
#include <easyx.h>

// ½ÇÉ«¿ØÖÆ
class MoveControl;
class SkillControl;

class Control
{
protected:
	MoveControl* move_control_ = nullptr;
	SkillControl* skill_control_ = nullptr;
public:
	Control(MoveControl* move_control, SkillControl* skill_control = nullptr)
		:move_control_(move_control), skill_control_(skill_control)
	{ }
	virtual ~Control();
	virtual void on_input(const ExMessage& msg);
};

class MoveControl
{
protected:
	Character* parent_ = nullptr;
public:
	MoveControl(Character* parent):parent_(parent){}
	virtual void on_input(const ExMessage& msg);
};

class SkillControl
{
protected:
	Character* parent_ = nullptr;
public:
	SkillControl(Character* parent) :parent_(parent) {}
	virtual void on_input(const ExMessage& msg);
};