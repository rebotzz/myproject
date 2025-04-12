#pragma once
#include "scene.h"
#include "timer.h"

#include <string>

// 即使没有好的框架，即使代码杂乱，但先实现功能


// 将场景拆分为对话、游戏两个部分
// 对话属于场景？还是对话在场景之上？
// 先选用 对话在场景之上
class DayScene : public Scene
{
public:

	void on_enter() override;// 场景切换资源准备工作
	void on_exit() override;
};

class NightScene : public Scene
{
public:

	void on_enter() override;// 场景切换资源准备工作
	void on_exit() override;
};

class TransitionScene : public Scene
{
private:
	Timer timer;				// 场景过度等待时间
	std::string text;			// 场景过度显示文本
	std::string next_scene;		// 即将过度场景

public:
	TransitionScene();

	void on_input(const SDL_Event& evnet) override;
	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_enter() override;// 场景切换资源准备工作
	void on_exit() {}

	void set_wait_time(float delta);
	void set_text(const std::string& val);
	void set_next_scene(const std::string& id);
};
