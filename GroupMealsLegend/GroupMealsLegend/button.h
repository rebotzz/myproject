#pragma once
#include "region.h"
#include <functional>
#include <string>

// 按钮
class Button : public Region
{
private:
	SDL_Texture* texture = nullptr;					// 按键图片
	std::string desc;								// 按键描述
	std::function<void(void)> on_click;				// 按键触发逻辑
	bool is_button_down = false;					// 是否按下
	bool is_enabled = true;							// 按键是否启用

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

