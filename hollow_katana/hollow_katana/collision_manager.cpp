#include <easyx.h>
#include "collision_manager.h"


CollisionManager* CollisionManager::manager = nullptr;
CollisionManager::CollisionManager() = default;
CollisionManager::~CollisionManager() = default;

CollisionManager* CollisionManager::instance()
{
	if (nullptr == manager)
		manager = new CollisionManager();

	return manager;
}

CollisionBox* CollisionManager::create_collision_box()
{
	CollisionBox* collision_box = new CollisionBox;
	collision_box_list.push_back(collision_box);

	return collision_box;
}

void CollisionManager::destroy_collision_box(CollisionBox* collision_box)
{
	collision_box_list.erase(std::remove(collision_box_list.begin(),
		collision_box_list.end(),collision_box), collision_box_list.end());

	delete collision_box;
}

void CollisionManager::process_collide()
{
	// Ë«²ãforÑ­»·¼ì²âÅö×²  src (È¥Åö×²)--> dst
	for (const auto& collision_box_src : collision_box_list)
	{
		if (!collision_box_src->enabled || collision_box_src->layer_dst == CollisionLayer::None)
			continue;

		for (const auto& collision_box_dst : collision_box_list)
		{
			if (!collision_box_dst->enabled || collision_box_src == collision_box_dst 
				|| collision_box_src->layer_dst != collision_box_dst->layer_src)
				continue;

			bool x_can_collide = (max(collision_box_src->position.x + collision_box_src->size.x / 2,
				collision_box_dst->position.x + collision_box_dst->size.x / 2)
				- min(collision_box_src->position.x - collision_box_src->size.x / 2,
					collision_box_dst->position.x - collision_box_dst->size.x / 2)) <= (collision_box_dst->size.x + collision_box_src->size.x);

			bool y_can_collide = (max(collision_box_src->position.y + collision_box_src->size.y / 2,
				collision_box_dst->position.y + collision_box_dst->size.y / 2)
				- min(collision_box_src->position.y - collision_box_src->size.y / 2,
					collision_box_dst->position.y - collision_box_dst->size.y / 2)) <= (collision_box_dst->size.y + collision_box_src->size.y);

			if (x_can_collide && y_can_collide && collision_box_src->on_collision)
				collision_box_dst->on_collision();
		}
	}
}

void CollisionManager::on_debug_render()
{
	// äÖÈ¾Åö×²¾ØÐÎ
	for (const auto* collision_box : collision_box_list)
	{
		collision_box->enabled ? setlinecolor(RGB(255, 255, 255)) : setlinecolor(RGB(255, 0, 0));
		rectangle((int)(collision_box->position.x - collision_box->size.x / 2), 
			(int)(collision_box->position.y - collision_box->size.y / 2),
			(int)(collision_box->position.x + collision_box->size.x / 2),
			(int)(collision_box->position.y + collision_box->size.y / 2));
	}
}