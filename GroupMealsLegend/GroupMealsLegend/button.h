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


// 重做按钮
class ButtonRedo : public Button
{
public:
	ButtonRedo():Button(815, 565) 
	{
		set_describe(u8"重做");
	}
};

// 调制按钮/停止摇晃按钮/上酒按钮  可以归为一类，用不同状态区分
class ButtonModulate : public Button
{
private:
	enum class Status
	{
		Init, Doing, Done	// 初始状态->调制中（摇晃）->制作完成（停止摇晃）->提交
	};
private:
	Status status = Status::Init;
public:
	ButtonModulate() :Button(1115, 565)
	{
		set_describe(u8"调制");
	}

	void on_render(SDL_Renderer* renderer) override;
};