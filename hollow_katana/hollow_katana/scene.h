#pragma once
#include <string>
#include <easyx.h>
#include "camera.h"

// ��������
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


// bug:Ϊʲôʹ�ö༶�̳г�����?
// ����༶�̳�,���ﲻ�̳�,����װ		
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