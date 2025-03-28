#pragma once
#include "boundle.h"
#include "resources_manager.h"

class ColaBoundle : public Boundle
{
public:
	ColaBoundle(int x, int y) :Boundle(x, y, 120, 124 , ResMgr::instance()->find_texture("cola_boundle"), Meal::Cola) {};
	~ColaBoundle() = default;
};

class SpriteBoundle : public Boundle
{
public:
	SpriteBoundle(int x, int y) :Boundle(x, y, 120, 124, ResMgr::instance()->find_texture("sprite_boundle"), Meal::Sprite) {};
	~SpriteBoundle() = default;
};

class TbBoundle : public Boundle
{
public:
	TbBoundle(int x, int y) :Boundle(x, y, 112, 96, ResMgr::instance()->find_texture("tb_boundle"), Meal::TakeoutBox) {};
	~TbBoundle() = default;
};

class MbBoxBoundle : public Boundle
{
public:
	MbBoxBoundle(int x, int y) :Boundle(x, y, 160, 88, ResMgr::instance()->find_texture("mb_box_boundle"), Meal::MeatBall_Box) {};
	~MbBoxBoundle() = default;
};

class BcBoxBoundle : public Boundle
{
public:
	BcBoxBoundle(int x, int y) :Boundle(x, y, 160, 88, ResMgr::instance()->find_texture("bc_box_boundle"), Meal::BraisedChicken_Box) {};
	~BcBoxBoundle() = default;
};

class RcpBoxBoundle : public Boundle
{
public:
	RcpBoxBoundle(int x, int y) :Boundle(x, y, 160, 88, ResMgr::instance()->find_texture("rcp_box_boundle"), Meal::RedCookedPork_Box) {};
	~RcpBoxBoundle() = default;
};

