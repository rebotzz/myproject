#pragma once
#include "region.h"
#include <functional>
#include <string>

// һ�㰴��
class Button : public Region
{
private:
	SDL_Texture* texture = nullptr;					// ����ͼƬ
	std::string desc;								// ��������
	std::function<void(void)> on_click;				// ���������߼�
	bool is_button_down = false;					// �Ƿ���
	bool is_enabled = true;							// �����Ƿ�����

public:
	Button() = default;
	Button(int x, int y);

	void set_describe(const std::string& str);
	void set_on_click(const std::function<void(void)>& callback);
	void set_texture(SDL_Texture* tex);
	void set_enable(bool flag);

	virtual void on_update(float delta) override;
	virtual void on_render(SDL_Renderer* renderer) override;
	virtual void on_cursor_up() override;
	virtual void on_cursor_down() override;
};

// ���ְ���
class ButtonText : public Button
{
public:
	SDL_Color color = { 255,255,255,255 };
public:
	ButtonText(int x, int y) : Button(x, y) { set_texture(nullptr); }
	ButtonText():Button(0, 0) { set_texture(nullptr); }

	void set_color(SDL_Color _color) { color = _color; }
	virtual void on_render(SDL_Renderer* renderer) override;
};