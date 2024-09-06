#pragma once
#include <vector>
#include "collision_box.h"


// 碰撞管理器: 单例+工厂模式
class CollisionManager
{
private:
	static CollisionManager* manager;
	std::vector<CollisionBox*> collision_box_list;

private:
	CollisionManager();
	~CollisionManager();

public:
	static CollisionManager* instance();
	CollisionBox* create_collision_box();
	void destroy_collision_box(CollisionBox* collision_box);
	void process_collide();
	void on_debug_render();

};