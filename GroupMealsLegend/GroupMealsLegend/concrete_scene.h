#pragma once
#include "scene.h"
#include "timer.h"

#include <string>

// ��ʹû�кõĿ�ܣ���ʹ�������ң�����ʵ�ֹ���


// ���������Ϊ�Ի�����Ϸ��������
// �Ի����ڳ��������ǶԻ��ڳ���֮�ϣ�
// ��ѡ�� �Ի��ڳ���֮��
class DayScene : public Scene
{
public:

	void on_enter() override;// �����л���Դ׼������
	void on_exit() override;
};

class NightScene : public Scene
{
public:

	void on_enter() override;// �����л���Դ׼������
	void on_exit() override;
};

class TransitionScene : public Scene
{
private:
	Timer timer;				// �������ȵȴ�ʱ��
	std::string text;			// ����������ʾ�ı�
	std::string next_scene;		// �������ȳ���

public:
	TransitionScene();

	void on_input(const SDL_Event& evnet) override;
	void on_update(float delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_enter() override;// �����л���Դ׼������
	void on_exit() {}

	void set_wait_time(float delta);
	void set_text(const std::string& val);
	void set_next_scene(const std::string& id);
};
