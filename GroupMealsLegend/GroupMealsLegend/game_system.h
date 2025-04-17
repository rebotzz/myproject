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

	void switch_bgm(const std::string& id);				// �������֣�ֻ��һ��
	void play_audio(const std::string& id);				// ��Ч�����Զ��
	// ��ϷĿ���趨���
	void set_coins_goal(int val);
	void set_drink_goal(const std::string& target);	
	void finish_goal();
	// �浵���
	void save_game();		
	void load_game();
};