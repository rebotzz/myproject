#pragma once
#include "region.h"

#include <string>

// �Ի�������	
// ��Ҫ����ɫ���� + ����� + ��ʷ�Ի���¼�� + ��Ч + ��Ļ���������Ч + �������Ӧ����һ������������һ���Ի���
class DialogueMgr
{
private:
	static DialogueMgr* manager;



private:
	DialogueMgr() = default;
	~DialogueMgr() = default;

public:
	static DialogueMgr* instance();


	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void parse();		// �����ű���ȷ��������ִ��ʲô

	void set_idx(int val);
	bool check_idx();
};