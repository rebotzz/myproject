#pragma once
#include "scene.h"
#include "timer.h"

#include <string>

// 即使没有好的框架，即使代码杂乱，但先实现功能

// 将场景拆分为对话、游戏两个部分
// 对话在场景之上

// 白天饭店场景
class DayScene : public Scene
{
private:
	Timer timer;

public:
	DayScene();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_enter() override;// 场景切换资源准备工作
	void on_exit() override;
};

// 夜晚酒吧场景
class NightScene : public Scene
{
public:

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_enter() override;
	void on_exit() override;
};

class TransitionScene : public Scene
{
private:
	Timer timer;				// 场景过度等待时间
	std::string text;			// 场景过度显示文本
	std::string next_scene;		// 即将过度场景
	int counter = 0;
	SDL_Texture* tex_bg;		// 背景图
	bool is_center = true;		// 文字是否在中心位置

public:
	TransitionScene();

	void on_input(const SDL_Event& evnet) override;
	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_enter() override;
	void on_exit() override;

	void set_wait_time(float delta);
	void set_text(const std::string& val);
	void set_next_scene(const std::string& id);

	void set_background(const std::string& tex_id);
	void set_text_position(const std::string& pos);
};
