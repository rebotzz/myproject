#pragma once
#include <vector>
#include "collision_box.h"


// ��ײ������: ����+����ģʽ
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