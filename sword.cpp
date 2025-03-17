#define _CRT_SECURE_NO_WARNINGS 1
#include"sword.h"
#include"collisionmanager.h"
#include"resourcesmanager.h"

Sword::Sword(const Vector2& position, bool move_left)
{
	animation.set_anchor_mode(Animation::AnchorMode::Centered);
	animation.set_interval(0.1f);
	animation.set_loop(true);
	animation.add_frame(ResourcesManager::instance()->find_atlas(
		move_left ? "sword_left" : "sword_right"
	));

	collision_box = CollisionManager::instance()->create_collision_box();
	collision_box->set_layer_src(CollisionLayer::None);
	collision_box->set_layer_dst(CollisionLayer::Player);
	collision_box->set_size({195,10});

	this->position = position;
	//this->is_vaild = true;
	this->velocity.x = move_left ? -SPEED_MOVE : SPEED_MOVE;
	this->velocity.y = 0;

}
Sword::~Sword()
{
	CollisionManager::instance()->destory_collision_box(collision_box);
}
void Sword::on_update(float delta) 
{
	position += velocity * delta;
	animation.set_position(position);
	collision_box->set_position(position);

	if (position.x < -200 || position.x >= getwidth() + 200)
		is_vaild = false;

	animation.on_update(delta);
}
void Sword::on_render()
{
	animation.on_render();
}
