#pragma once
#include "region.h"
#include <functional>
#include <string>

// ��ť
class Button : public Region
{
private:
	SDL_Texture* texture = nullptr;					// ����ͼƬ
	std::string desc;								// ��������
	std::function<void(void)> on_click;				// ���������߼�
	bool is_button_down = false;					// �Ƿ���

public:
	Button(int x, int y);
	Button(int x, int y, SDL_Texture* _texture, const std::function<void(void)>& callback) 
		:Region({x, y, 134, 46}), texture(_texture), on_click(callback) {}
	Button(int x, int y, const std::function<void(void)>& callback)
		:Region({ x, y, 134, 46 }), on_click(callback) {
	}

	void set_describe(const std::string& str);
	void set_on_click(const std::function<void(void)>& callback);

	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;
	virtual void on_cursor_down() override;
};


// ������ť
class ButtonRedo : public Button
{
public:
	ButtonRedo():Button(815, 565) 
	{
		set_describe(u8"����");
	}
};

// ���ư�ť/ֹͣҡ�ΰ�ť/�Ͼư�ť  ���Թ�Ϊһ�࣬�ò�ͬ״̬����
class ButtonModulate : public Button
{
private:
	enum class Status
	{
		Init, Doing, Done	// ��ʼ״̬->�����У�ҡ�Σ�->������ɣ�ֹͣҡ�Σ�->�ύ
	};
private:
	Status status = Status::Init;
public:
	ButtonModulate() :Button(1115, 565)
	{
		set_describe(u8"����");
	}

	void on_render(SDL_Renderer* renderer) override;
};