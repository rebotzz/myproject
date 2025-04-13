#pragma once
#include "SDL.h"
#include "timer.h"

#include <string>
#include <queue>

// �Ի�������	
// ��Ҫ����ɫ���� + ����� + ��ʷ�Ի���¼�� + ��Ч + ��Ļ���������Ч + �������Ӧ����һ������������һ���Ի���
// �۲���ģʽ��
// ��Ϊ�����Ϸ�ɶԻ��������£����ԶԻ����������ִ�й��½ű�

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
		SDL_Rect rect = { 0 };
		Color color = Color::C1;
		std::string img;

		//bool finished = false;

	public:
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

	DialogBox dialog_box;
	std::string script_id = "script_1";		// ִ�нű�������Դ������
	int idx = 0;
	bool condition = true;					// ���������ű��������Ƿ����
	std::string goal;						// ���������ű�ǰ��Ҫ��ɵ�Ŀ��

	//DialogHistory dialog_history;

private:
	DialogMgr() = default;
	~DialogMgr() = default;

public:
	static DialogMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void parse();				// �����ű���ȷ��������ִ��ʲô

	void set_idx(int val);
	bool check_idx() const;

	void set_script_id(const std::string& id);

	void finish_goal();
	const std::string& get_goal() const;

};