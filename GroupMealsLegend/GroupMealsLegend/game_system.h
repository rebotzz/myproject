#pragma once
#include "SDL.h"

#include <string>

// 游戏系统，最上层，调用所有管理器
class GameSystem
{
public:
	// 游戏模式
	// 或许对话模式和操作模式可以用不同的游戏场景实现。看起来相同，但是细节不同
	enum class Mode
	{
		Dialogue,	// 对话模式
		Operator	// 操作模式
	};
private:
	static GameSystem* manager;
	Mode mode = Mode::Operator;

	SDL_Window* win = nullptr;
	SDL_Renderer* renderer = nullptr;
	const int WINDOW_W = 1280, WINDOW_H = 720;
	const int FPS = 30;
	std::string bgm;
	int bgm_channel = -1;

private:
	GameSystem() = default;
	~GameSystem() = default;

public:
	static GameSystem* instance();

	void set_mode(Mode val);
	Mode get_mode() const;
	void start();

	void switch_bgm(const std::string& id);				// 背景音乐，只有一个
	void play_audio(const std::string& id);				// 音效，可以多个
	// 游戏目标设定相关
	void set_coins_goal(int val);
	void set_drink_goal(const std::string& target);	
	void finish_goal();
	// 存档相关
	void save_game();		
	void load_game();
};