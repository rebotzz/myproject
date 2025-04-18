#pragma once
#include "bartend_material.h"
#include "bartend_bottle.h"
#include "bartend_meun.h"
#include "button.h"
#include "timer.h"

// 调酒系统统一管理。避免各个组件间相互调用
class BartendSystem
{
public:
	enum class Status
	{
		Init, Doing, Done	// 初始状态->调制中（摇晃）->制作完成（停止摇晃）->提交
	};

private:
	static BartendSystem* manager;

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
	// 调酒指南
	BartendMeun bartendmeun;
	// 功能按钮; 不用具体类，注册回调就行
	Button button_redo;
	Button button_modulate;

	bool valid = false;						// 是否启用
	Status status = Status::Init;			// 用于辅助描述按键状态
	Timer timer_button;						// 更新按键状态
	Region2 btc_bottom;						// 绘制底部图片

private:
	BartendSystem();		
	~BartendSystem() = default;

public:
	static BartendSystem* instance();

	void open();
	void close();
	
	// 被具体场景调用
	void on_update(float delta);

	void reset();
	void modulate();
	void enable_meun(bool flag);
};