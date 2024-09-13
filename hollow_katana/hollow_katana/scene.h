#pragma once
#include <string>
#include <easyx.h>
#include "camera.h"

// 场景基类
class Scene
{
public:
	Scene() = default;
	~Scene() = default;
	virtual void on_enter() = 0;
	virtual void on_input(const ExMessage& msg) { };
	virtual void on_update(float delta_time) { };
	virtual void on_render() { };
	virtual void on_exit() { };
};


// bug:为什么使用多级继承出错了?
// 避免多级继承,这里不继承,用组装		
class GameScene
{
protected:
	IMAGE* background = nullptr;
	std::wstring bgm;

public:
	GameScene(IMAGE* background, const std::wstring bgm)
	{
		this->background = background;
		this->bgm = bgm;
	}
	~GameScene() = default;
	void on_enter();
	void on_input(const ExMessage& msg);
	void on_update(float delta_time);
	void on_render();
	void on_exit();
	void render_background();
};