#include "dialogue_manager.h"


DialogueMgr* DialogueMgr::manager = nullptr;

DialogueMgr* DialogueMgr::instance()
{
	if (nullptr == manager)
	{
		manager = new DialogueMgr();
	}
	return manager;
}

void DialogueMgr::on_input(const SDL_Event& event)
{
	// �����/������Ӧ���Ի�/��ʷ
}
void DialogueMgr::on_update(float delta)
{
	// ��ʱ�����£�����0.5s�ε������ı���̬����
}
void DialogueMgr::on_render(SDL_Renderer* renderer)
{
	// ��Ⱦ��ɫ���棬������ı�
}

void DialogueMgr::parse()		
{
	// �����ű���ȷ��������ִ��ʲô

	// if(����)�л�ģʽ���趨����Ŀ�ꡣ�л�������������Ч
	// if(�԰�)�����Ի�
}
void DialogueMgr::set_idx(int val)
{

}
bool DialogueMgr::check_idx()
{

}