#pragma once
#include"collisionbox.h"
#include"animation.h"
#include"Vector2.h"

class Barb
{
public:
	Barb();
	~Barb();

	void on_update(float delta);
	void on_render();

	void set_position(const Vector2& pos)
	{
		this->base_position = pos;
		this->current_position = pos;
	}
	bool check_vaild()
	{
		return is_vaild;
	}
private:
	enum class Stage
	{
		Idle,
		Aim,
		Dash,
		Break
	};
private:
	const float SPEED_DASH = 1500.0f;

private:
	Timer timer_idle;
	Timer timer_aim;
	int diff_period = 0;
	float total_delta_time = 0;
	bool is_vaild = true;

	Vector2 base_position;
	Vector2 current_position;
	Vector2 velocity;

	Animation animation_loose;
	Animation animation_break;
	Animation* current_animation = nullptr;

	Stage stage = Stage::Idle;
	CollisionBox* collision_box = nullptr;

private:
	void on_break();
};