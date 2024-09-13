#pragma once
#include <string>
#include <easyx.h>
#include "camera.h"

// ³¡¾°»ùÀà
class Scene
{
public:
	Scene() = default;
	~Scene() = default;
	virtual void on_enter() = 0;
	virtual void on_input(const ExMessage& msg) = 0;
	virtual void on_update(float delta_time) = 0;
	virtual void on_render() = 0;
	virtual void on_exit() = 0;
};


class GameScene : Scene
{
public:
	IMAGE* background = nullptr;
	std::wstring bgm;

public:
	GameScene() = default;
	~GameScene() = default;
	virtual void on_enter()override;
	virtual void on_input(const ExMessage& msg)override;
	virtual void on_update(float delta_time)override;
	virtual void on_render()override;
	virtual void on_exit()override;
	void render_background();
};