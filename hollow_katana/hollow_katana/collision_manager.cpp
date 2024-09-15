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
		collision_box_list.end(), collision_box), collision_box_list.end());

	delete collision_box;
}

void CollisionManager::process_collide()
{
	// Ë«²ãforÑ­»·¼ì²âÅö×²  src (È¥Åö×²)--> dst
	for (const auto& collision_box_hit : collision_box_list)
	{
		if (!collision_box_hit->enabled || collision_box_hit->layer_dst == CollisionLayer::None)
			continue;

		for (const auto& collision_box_hurt : collision_box_list)
		{
			if (!collision_box_hurt->enabled || collision_box_hit == collision_box_hurt
				|| !collision_box_hit->check_collision_layer_dst(collision_box_hurt->layer_src))
				continue;

			bool x_can_collide = (max(collision_box_hit->position.x + collision_box_hit->size.x / 2,
				collision_box_hurt->position.x + collision_box_hurt->size.x / 2)
				- min(collision_box_hit->position.x - collision_box_hit->size.x / 2,
					collision_box_hurt->position.x - collision_box_hurt->size.x / 2)) <= (collision_box_hurt->size.x + collision_box_hit->size.x);

			bool y_can_collide = (max(collision_box_hit->position.y + collision_box_hit->size.y / 2,
				collision_box_hurt->position.y + collision_box_hurt->size.y / 2)
				- min(collision_box_hit->position.y - collision_box_hit->size.y / 2,
					collision_box_hurt->position.y - collision_box_hurt->size.y / 2)) <= (collision_box_hurt->size.y + collision_box_hit->size.y);

			if (x_can_collide && y_can_collide)
			{
				collision_box_hit->set_trigger_layer(
					collision_box_hit->layer_dst & collision_box_hurt->layer_src);
				collision_box_hurt->set_trigger_layer(
					collision_box_hit->layer_dst & collision_box_hurt->layer_src);

				if (collision_box_hit->on_collision)
					collision_box_hit->on_collision();
				if (collision_box_hurt->on_collision)
					collision_box_hurt->on_collision();
			}
		}
	}
}

void CollisionManager::on_debug_render()
{
	// äÖÈ¾Åö×²¾ØÐÎ
	for (const auto* collision_box : collision_box_list)
	{
		collision_box->enabled ? setlinecolor(RGB(255, 0, 0)) : setlinecolor(RGB(0, 255, 255));
		rectangle((int)(collision_box->position.x - collision_box->size.x / 2),
			(int)(collision_box->position.y - collision_box->size.y / 2),
			(int)(collision_box->position.x + collision_box->size.x / 2),
			(int)(collision_box->position.y + collision_box->size.y / 2));
	}
}