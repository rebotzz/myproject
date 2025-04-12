#pragma once
#include "SDL.h"
#include "timer.h"

#include <string>
#include <queue>

// �Ի�������	
// ��Ҫ����ɫ���� + ����� + ��ʷ�Ի���¼�� + ��Ч + ��Ļ���������Ч + �������Ӧ����һ������������һ���Ի���
// �۲���ģʽ��

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
	std::string script_id;		// ����Դ������
	int idx = 0;

	bool condition = true;		// ���������ű��������Ƿ����
	std::string goal;			// ���������ű�ǰ��Ҫ��ɵ�Ŀ��

private:
	DialogMgr() = default;
	DialogMgr(const std::string& _script_id) :script_id(_script_id) { }
	~DialogMgr() = default;

public:
	static DialogMgr* instance();

	void on_input(const SDL_Event& event);
	void on_update(float delta);
	void on_render(SDL_Renderer* renderer);

	void parse();				// �����ű���ȷ��������ִ��ʲô

	void set_idx(int val);
	bool check_idx() const;

	void finish_goal();
	const std::string& get_goal() const;

private:
	//const std::string& get_next_text() const;
};