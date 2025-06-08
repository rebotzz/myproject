#pragma once
#include <easyx.h>

// ½ÇÉ«¿ØÖÆ
class Character;
class MoveControl;
class SkillControl;

class Control
{
protected:
	Character* parent_ = nullptr;
public:
	Control(Character* parent)
		:parent_(parent)
	{ }
	virtual void on_input(const ExMessage& msg);
};

class PlayerControlAWSD : public Control
{
public:
	PlayerControlAWSD(Character* parent)
		:Control(parent)
	{}
	virtual void on_input(const ExMessage& msg) override;
};

class PlayerControlArrow : public Control
{
public:
	PlayerControlArrow(Character* parent)
		:Control(parent)
	{}
	virtual void on_input(const ExMessage& msg) override;
};

class HornetControl : public Control
{
public:
	HornetControl(Character* parent)
		:Control(parent)
	{ }
	virtual void on_input(const ExMessage& msg) override;
};