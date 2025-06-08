#pragma once
#include <memory>
#include <vector>
#include "scene.h"
#include "scene_game.h"
#include "door.h"
#include "woodenman.h"
#include "platform.h"




// v1.3����:������,ľͷ��,ƽ̨������
// todo: 
// 1.���ӵ���,��ֹ��ɫ�ɳ���Ļ����
// 2.��������ʱ,������������,��ײ���巽��,ê��λ��,��������Ծ����(��ʱ����,�����ڱ����Ϸ��ʵ��)
// TODO:������ö�̬���أ��õ�ʱ���ڴ�����һ����ȫ�������ײ����ܶ���ײ�� | ���߳������زĽ�����أ��˳�����
class SceneGameChoice : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	GameScene game_scene;

	// �������ܵ���
	Door* door_hornet = nullptr;							
	Door* door_dragon_king = nullptr;
	Woodenman* woodenman = nullptr;
	std::vector<Platform*> platform_list;			// ��Ծƽ̨
	bool enabled_platforms = false;
	InteractProp prop_platform_switch;				// ƽ̨����
	Timer timer_platforms_switch;

public:
	SceneGameChoice();
	~SceneGameChoice()
	{}

	virtual void on_enter() override;
	virtual void on_input(const ExMessage& msg) override;
	virtual void on_update(float delta) override;
	virtual void on_render() override;
	virtual void on_exit() override;
};
