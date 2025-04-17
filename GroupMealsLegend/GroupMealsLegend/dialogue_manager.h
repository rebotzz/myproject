#pragma once
#include "SDL.h"
#include "timer.h"

#include <string>
#include <queue>
#include <array>

// �Ի�������	
// ��Ҫ����ɫ���� + ����� + �����Ӧ
// ��Ϊ�����Ϸ�ɶԻ��������£����ԶԻ����������ִ�й��½ű�
// todo:��ʷ�Ի���¼�� + ��Ч + ��Ļ���������Ч
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
		std::string text;					// �Ի��ı�
		SDL_Rect rect = { 0 };				// �Ի�������
		Color color = Color::C1;			// ��ɫ������ɫ
		std::string img;					// ��ɫ����
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

	DialogBox dialog_box;					// �Ի���
	std::string script_id;					// ִ�нű�������Դ������
	int idx = 0;							// ��ǰִ�нű����
	bool condition = true;					// ���������ű��������Ƿ����
	bool showing_tip = false;				// �Ƿ���ʾ��ʾ

private:
	DialogMgr() = default;
	~DialogMgr() = default;

public:
	static DialogMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	// �����ű���ȷ��������ִ��ʲô
	void parse();				

	// ��ִ�нű����
	bool check_idx() const;
	void set_idx(int val);
	void set_script_id(const std::string& id);
	int get_idx() const;
	const std::string& get_script_id() const;

	// ��ϷĿ���ɣ������ƽ����
	void finish_goal();
	void set_tips(const std::string& val);
	void enable_tips(bool flag);
};