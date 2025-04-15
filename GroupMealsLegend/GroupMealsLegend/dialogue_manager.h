#pragma once
#include "SDL.h"
#include "timer.h"

#include <string>
#include <queue>
#include <array>

// 对话管理器	
// 需要：角色立绘 + 聊天框 + 历史对话记录框 + 音效 + 字幕逐个出现特效 + 鼠标点击响应（下一条、加载完整一条对话）
// 观察者模式？
// 因为这个游戏由对话驱动故事，所以对话管理器兼顾执行故事脚本

class DialogMgr
{
private:
	class DialogBox
	{
	public:
		enum class Color
		{
			C1, C2, C3, C4, C5, C6, None
		};
	private:
		std::string text;
		SDL_Rect rect = { 0 };
		Color color = Color::C1;
		std::string img;
		std::array<SDL_Color, static_cast<int>(Color::None)> color_map;

	public:
		DialogBox();
		void on_input(const SDL_Event& event);
		void on_update(float delta);
		void on_render(SDL_Renderer* renderer);
		void set_dialog(const std::string& text, const std::string& img, Color color);
	};

	//class DialogHistory 
	//{
	//private:
	//	std::queue<std::string> history;
	//	SDL_Rect rect;
	//	bool valid = false;

	//public:
	//	void on_input(const SDL_Event& event);
	//	void on_update(float delta);
	//	void on_render(SDL_Renderer* renderer);
	//	const SDL_Rect& get_rect() const;
	//};

private:
	static DialogMgr* manager;

	DialogBox dialog_box;					// 对话框
	std::string script_id = "script_1";		// 执行脚本，在资源管理中
	int idx = 0;							// 当前执行脚本序号
	bool condition = true;					// 继续解析脚本的条件是否完成
	bool showing_tip = false;				// 是否显示提示

	//DialogHistory dialog_history;

private:
	DialogMgr() = default;
	~DialogMgr() = default;

public:
	static DialogMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void parse();				// 解析脚本，确定接下来执行什么

	void set_idx(int val);
	bool check_idx() const;
	void set_script_id(const std::string& id);
	void finish_goal();
	void set_tips(const std::string& val);
	void enable_tips(bool flag);
};