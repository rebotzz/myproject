#pragma once
#include <string>
#include <easyx.h>
#include "timer.h"

// bug:为什么使用多级继承出错了?
// 避免多级继承,这里不继承,用组装		
class GameScene
{
protected:
	IMAGE* background = nullptr;
	std::wstring bgm;
	Timer timer_player_dead_text;
	Timer timer_cd;
	bool can_reverse_time = false;
	bool is_reverse_time = false;
	bool is_reverse_key_down = false;

public:
	GameScene(IMAGE* background, const std::wstring bgm);
	~GameScene() = default;
	void on_enter();
	void on_input(const ExMessage& msg);
	void on_update(float delta_time);
	void on_render();
	void on_exit();
	void render_background();
	void clear_record();
	void stop_record();
	bool is_reverse() { return is_reverse_time; }
};