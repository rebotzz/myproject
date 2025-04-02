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
	// 鼠标点击/滚动响应，对话/历史
}
void DialogueMgr::on_update(float delta)
{
	// 定时器更新，给个0.5s滴滴声，文本动态出现
}
void DialogueMgr::on_render(SDL_Renderer* renderer)
{
	// 渲染角色立绘，聊天框，文本
}

void DialogueMgr::parse()		
{
	// 解析脚本，确定接下来执行什么

	// if(命令)切换模式，设定结束目标。切换场景、播放音效
	// if(对白)继续对话
}
void DialogueMgr::set_idx(int val)
{

}
bool DialogueMgr::check_idx()
{

}