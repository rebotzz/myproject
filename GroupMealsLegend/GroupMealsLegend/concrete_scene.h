#pragma once
#include "scene.h"
#include "timer.h"

#include <string>

// ��ʹû�кõĿ�ܣ���ʹ�������ң�����ʵ�ֹ���

// ���������Ϊ�Ի�����Ϸ��������
// �Ի��ڳ���֮��

// ���췹�곡��
class DayScene : public Scene
{
private:
	Timer timer;

public:
	DayScene();

	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_enter() override;// �����л���Դ׼������
	void on_exit() override;
};

// ҹ��ưɳ���
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
	Timer timer;				// �������ȵȴ�ʱ��
	std::string text;			// ����������ʾ�ı�
	std::string next_scene;		// �������ȳ���
	int counter = 0;
	SDL_Texture* tex_bg;		// ����ͼ
	bool is_center = true;		// �����Ƿ�������λ��

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
