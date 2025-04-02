#pragma once
#include "region.h"

#include <string>

// 对话管理器	
// 需要：角色立绘 + 聊天框 + 历史对话记录框 + 音效 + 字幕逐个出现特效 + 鼠标点击响应（下一条、加载完整一条对话）
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

	void parse();		// 解析脚本，确定接下来执行什么

	void set_idx(int val);
	bool check_idx();
};