#pragma once
#include"Vector2.h"
#include"collisionlayer.h"

#include<functional>

class CollisionManager;

class CollisionBox
{
	friend class CollisionManager;

private:
	CollisionBox()=default;
	~CollisionBox()=default;
public:
	void set_enabled(bool flag)
	{
		enabled = flag;
	}
	void set_position(const Vector2& position)
	{
		this->position = position;
	}
	void set_size(const Vector2& size)
	{
		this->size = size;
	}
	const Vector2& get_size()
	{
		return size;
	}
	void set_layer_src(CollisionLayer layer)
	{
		layer_src = layer;
	}
	void set_layer_dst(CollisionLayer layer)
	{
		layer_dst = layer;
	}
	void set_on_collide(std::function<void()> on_collide)
	{
		this->on_collide = on_collide;
	}
private:
	Vector2 position;
	Vector2 size;
	bool enabled=false;
	std::function<void()> on_collide=nullptr;
	CollisionLayer layer_src = CollisionLayer::None;
	CollisionLayer layer_dst = CollisionLayer::None;
};