#pragma once
#include "region.h"

// 对话管理器
class DialogueMgr
{
private:
	static DialogueMgr* manager;

private:
	DialogueMgr();
	~DialogueMgr();

public:
	static DialogueMgr* instance();


};