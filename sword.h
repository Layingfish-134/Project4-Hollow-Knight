#pragma once
#include"collisionbox.h"
#include"Vector2.h"
#include"animation.h"

class Sword
{
public:
	Sword(const Vector2 & position,bool move_left);
	~Sword();

	void on_update(float delta);
	void on_render();

	bool check_vaild()
	{
		return is_vaild;
	}
private:
	const float SPEED_MOVE = 1250.0f;
private:
	Vector2 position;
	Vector2 velocity;
	bool is_vaild = true;
	CollisionBox* collision_box=nullptr;
	Animation animation;
};