#pragma once
#include "bartend_material.h"
#include "bartend_tool.h"

// 调酒系统统一管理。避免各个组件间相互调用
class BartendSystem
{
	//RegionMgr::instance()->add("Adelhyde", new Adelhyde, 0);
	//RegionMgr::instance()->add("BronsonExt", new BronsonExt, 0);
	//RegionMgr::instance()->add("PwdDelta", new PwdDelta, 0);
	//RegionMgr::instance()->add("Flanergide", new Flanergide, 0);
	//RegionMgr::instance()->add("Karmotrine", new Karmotrine, 0);
	//RegionMgr::instance()->add("Ice", new Ice, 0);
	//RegionMgr::instance()->add("Ageing", new Ageing, 0);

	//RegionMgr::instance()->add("ButtonRedo", new ButtonRedo, 0);
	//RegionMgr::instance()->add("ButtonModulate", new ButtonModulate, 0);
	//RegionMgr::instance()->add("BartendBottle", new BartendBottle, 0);

private:
	// 原料
	Adelhyde	 adelhyde;
	BronsonExt	 bronsonext;
	PwdDelta	 pwddelta;
	Flanergide	 flanergide;
	Karmotrine	 karmotrine;
	Ice			 ice;
	Ageing		 ageing;

	// 调酒瓶子
	BartendBottle bartendbottle;

	// 功能按钮
	ButtonRedo buttonredo;
	ButtonModulate buttonmodulate;


public:
	BartendSystem();
	~BartendSystem();

	void on_input();
	void on_update();
	void on_render();
};