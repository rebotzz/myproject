#pragma once
#include "SDL.h"

#include <string>

// ��Ϸϵͳ�����ϲ㣬�������й�����
class GameSystem
{
public:
	// ��Ϸģʽ
	// ����Ի�ģʽ�Ͳ���ģʽ�����ò�ͬ����Ϸ����ʵ�֡���������ͬ������ϸ�ڲ�ͬ
	enum class Mode
	{
		Dialogue,	// �Ի�ģʽ
		Operator	// ����ģʽ
	};
private:
	static GameSystem* manager;
	Mode mode = Mode::Operator;

	SDL_Window* win = nullptr;
	SDL_Renderer* renderer = nullptr;
	const int WINDOW_W = 1280, WINDOW_H = 720;
	const int FPS = 60;
	std::string bgm;

private:
	GameSystem() = default;
	~GameSystem() = default;

public:
	static GameSystem* instance();

	void set_mode(Mode val);
	Mode get_mode() const;
	void start();

	void switch_bgm(const std::string& id);
	void set_goal(int val);
	void finish_goal();
};