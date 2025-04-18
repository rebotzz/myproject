#pragma once
#include "SDL.h"
#include "timer.h"

#include <string>
#include <queue>
#include <array>

// 对话管理器	
// 需要：角色立绘 + 聊天框 + 点击响应
// 因为这个游戏由对话驱动故事，所以对话管理器兼顾执行故事脚本
// todo:历史对话记录框 + 音效 + 字幕逐个出现特效
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
		std::string text;					// 对话文本
		SDL_Rect rect = { 0 };				// 对话框区域
		Color color = Color::C1;			// 角色名字颜色
		std::string img;					// 角色立绘
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
	std::string script_id;					// 执行脚本，在资源管理中
	int idx = 0;							// 当前执行脚本序号
	bool condition = true;					// 继续解析脚本的条件是否完成
	bool showing_tip = false;				// 是否显示提示

private:
	DialogMgr() = default;
	~DialogMgr() = default;

public:
	static DialogMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	// 解析脚本，确定接下来执行什么
	void parse();				

	// 与执行脚本相关
	bool check_idx() const;
	void set_idx(int val);
	void set_script_id(const std::string& id);
	int get_idx() const;
	const std::string& get_script_id() const;

	// 游戏目标达成，继续推进相关
	void finish_goal();
	void set_tips(const std::string& val);
	void enable_tips(bool flag);
};