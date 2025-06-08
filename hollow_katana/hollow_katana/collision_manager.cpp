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
	for (const auto& hit_box : collision_box_list)
	{
		if (!hit_box->enabled || hit_box->layer_dst == CollisionLayer::None)
			continue;

		for (const auto& hurt_box : collision_box_list)
		{
			if (!hurt_box->enabled || hit_box == hurt_box
				|| !hit_box->check_collision_layer_dst(hurt_box->layer_src))
				continue;

			bool x_can_collide = (max(hit_box->position.x + hit_box->size.x / 2,
				hurt_box->position.x + hurt_box->size.x / 2)
				- min(hit_box->position.x - hit_box->size.x / 2,
					hurt_box->position.x - hurt_box->size.x / 2)) <= (hurt_box->size.x + hit_box->size.x);

			bool y_can_collide = (max(hit_box->position.y + hit_box->size.y / 2,
				hurt_box->position.y + hurt_box->size.y / 2)
				- min(hit_box->position.y - hit_box->size.y / 2,
					hurt_box->position.y - hurt_box->size.y / 2)) <= (hurt_box->size.y + hit_box->size.y);

			if (x_can_collide && y_can_collide)
			{
				hit_box->set_trigger_layer(
					hit_box->layer_dst & hurt_box->layer_src);
				hurt_box->set_trigger_layer(
					hit_box->layer_dst & hurt_box->layer_src);

				if (hit_box->on_collision)
					hit_box->on_collision(hurt_box);
				if (hurt_box->on_collision)
					hurt_box->on_collision(hit_box);
			}
		}
	}
}

void CollisionManager::on_debug_render()
{
	// äÖÈ¾Åö×²¾ØÐÎ
	setlinestyle(PS_SOLID, 1);
	for (const auto* collision_box : collision_box_list)
	{
		collision_box->enabled ? setlinecolor(RGB(255, 0, 0)) : setlinecolor(RGB(0, 255, 255));
		rectangle((int)(collision_box->position.x - collision_box->size.x / 2),
			(int)(collision_box->position.y - collision_box->size.y / 2),
			(int)(collision_box->position.x + collision_box->size.x / 2),
			(int)(collision_box->position.y + collision_box->size.y / 2));
	}
}