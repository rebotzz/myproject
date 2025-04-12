#pragma once
#include "SDL.h"
#include "timer.h"

#include <string>
#include <queue>

// 对话管理器	
// 需要：角色立绘 + 聊天框 + 历史对话记录框 + 音效 + 字幕逐个出现特效 + 鼠标点击响应（下一条、加载完整一条对话）
// 观察者模式？

class DialogMgr
{
private:
	class DialogBox 
	{
	public:
		enum class Color
		{
			C1, C2, C3
		};
	private:
		std::string text;
		SDL_Rect rect;
		//bool finished = false;
		Color color = Color::C1;
		std::string img_id;



	public:
		void on_input(const SDL_Event& event);
		void on_update(float delta);
		void on_render(SDL_Renderer* renderer);
		const SDL_Rect& get_rect() const;
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

	DialogBox dialog_box;
	DialogHistory dialog_history;
	std::string script_id;		// 在资源管理中
	int idx = 0;

	bool condition = true;		// 继续解析脚本的条件是否完成
	std::string goal;			// 继续解析脚本前需要完成的目标

private:
	DialogMgr() = default;
	DialogMgr(const std::string& _script_id) :script_id(_script_id) { }
	~DialogMgr() = default;

public:
	static DialogMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void parse();				// 解析脚本，确定接下来执行什么

	void set_idx(int val);
	bool check_idx() const;

	void finish_goal();
	const std::string& get_goal() const;

private:
	//const std::string& get_next_text() const;
};