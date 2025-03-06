#pragma once
#include"collisionbox.h"

#include<vector>

class CollisionManager
{

public:
	static CollisionManager* instance();

	CollisionBox* create_collision_box();
	void destory_collision_box(CollisionBox* collision_box);

	void process_collision();
	void on_debug_render();

private:
	CollisionManager()=default;
	~CollisionManager()=default;

private:
	static CollisionManager* collision_manager;

	std::vector<CollisionBox*> collision_box_list;
};
