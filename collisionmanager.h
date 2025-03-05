#pragma once
#include"Vector2.h"
#include"collisionlayer.h"

#include<functional>

class CollsionManager;

class CollsionBox
{
	friend class CollsionManager;

private:
	CollsionBox()=default;
	~CollsionBox()=default;
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
	void set_layer_src(CollsionLayer layer)
	{
		layer_src = layer;
	}
	void set_layer_dst(CollsionLayer layer)
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
	CollsionLayer layer_src = CollsionLayer::None;
	CollsionLayer layer_dst = CollsionLayer::None;
};