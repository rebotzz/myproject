#pragma once
#include "region.h"

// �Ի�������
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