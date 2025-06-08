#pragma once
#include <memory>
#include <vector>
#include "scene.h"
#include "scene_game.h"
#include "door.h"
#include "woodenman.h"
#include "platform.h"




// v1.3新增:传送门,木头人,平台，开关
// todo: 
// 1.增加挡板,防止角色飞出屏幕顶部
// 2.重力更改时,调整动画方向,碰撞箱体方向,锚点位置,或许还有跳跃方向(暂时废弃,或许在别的游戏中实现)
// TODO:场景最好动态加载，用到时候在创建，一股脑全创建容易产生很多碰撞箱 | 或者场景内素材进入加载，退出销毁
class SceneGameChoice : public Scene
{
private:
	Vector2 pos_player = { 200, 620 };
	GameScene game_scene;

	// 交互功能道具
	Door* door_hornet = nullptr;							
	Door* door_dragon_king = nullptr;
	Woodenman* woodenman = nullptr;
	std::vector<Platform*> platform_list;			// 跳跃平台
	bool enabled_platforms = false;
	InteractProp prop_platform_switch;				// 平台开关
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
