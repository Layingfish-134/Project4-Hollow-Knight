#define _CRT_SECURE_NO_WARNINGS 1
#include"collisionmanager.h"

#include<algorithm>
#include<graphics.h>

CollisionManager* CollisionManager::collision_manager = nullptr;

CollisionManager* CollisionManager::instance()
{
	if (!collision_manager)
		collision_manager = new CollisionManager;
	return collision_manager;
}

CollisionBox* CollisionManager::create_collision_box()
{
	CollisionBox* collision_box = new CollisionBox;

	collision_box_list.push_back(collision_box);

	return collision_box;
}
void CollisionManager::destory_collision_box(CollisionBox* collision_box)
{
	collision_box_list.erase(std::remove(collision_box_list.begin(),
		collision_box_list.end(), collision_box), collision_box_list.end());

	delete collision_box;

	return;
}
void CollisionManager::process_collision()
{
	for (const CollisionBox* collision_box_src : collision_box_list)
	{
		if (!collision_box_src->enabled || 
			collision_box_src->layer_dst == CollisionLayer::None)
		{
			continue;
		}

		for (const CollisionBox* collision_box_dst : collision_box_list)
		{
			if (!collision_box_dst->enabled || collision_box_dst == collision_box_src
				|| collision_box_src->layer_dst != collision_box_dst->layer_src)
				continue;

			bool is_collide_x = (max(collision_box_dst->position.x+collision_box_dst->size.x/2
				,collision_box_src->position.x+collision_box_src->size.x/2)-
				min(collision_box_dst->position.x-collision_box_dst->size.x/2
					,collision_box_src->position.x-collision_box_src->size.x/2)
				<=(collision_box_dst->size.x+collision_box_src->size.x));

			bool is_collide_y = (max(collision_box_dst->position.y + collision_box_dst->size.y / 2
				, collision_box_src->position.y + collision_box_src->size.y / 2) -
				min(collision_box_dst->position.y - collision_box_dst->size.y / 2
					, collision_box_src->position.y- collision_box_src->size.y / 2)
				<= (collision_box_dst->size.y + collision_box_src->size.y));


			if (is_collide_x && is_collide_y && collision_box_dst->on_collide)
			{
				collision_box_dst->on_collide();
			}
		}

	}
}
void CollisionManager::on_debug_render()
{
	for (const CollisionBox* collision_box : collision_box_list)
	{
		setlinecolor(collision_box->enabled ? RGB(255, 195, 195) : RGB(115, 115, 175));
		rectangle(
			(int)(collision_box->position.x - collision_box->size.x / 2),
			(int)(collision_box->position.y - collision_box->size.y / 2),
			(int)(collision_box->position.x + collision_box->size.x / 2),
			(int)(collision_box->position.y + collision_box->size.y / 2));
	}
}
