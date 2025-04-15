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
	bool is_enabled = true;							// �����Ƿ�����

public:
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

