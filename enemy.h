#pragma once
#include"barb.h"
#include"sword.h"
#include"charactor.h"

class Enemy:public Charactor
{
public:
	Enemy();
	~Enemy();

	void on_update(float delta) override;
	void on_render() override;

	void on_hurt()override;

public:
	void set_facing_left(bool flag)
	{
		is_facing_left = flag;
	}
	bool get_facing_left() const
	{
		return is_facing_left;
	}
	void set_dash_in_air(bool flag)
	{
		is_dash_in_air = flag;
	}
	bool get_dash_in_air()const
	{
		return is_dash_in_air;
	}
	void set_dash_on_floor(bool flag)
	{
		is_dash_on_floor = flag;
	}
	bool get_dash_on_floor()const
	{
		return is_dash_on_floor;
	}
	void set_throwing_silk(bool flag)
	{
		is_throwing_silk = flag;
	}
	bool get_throwing_silk()
	{
		return is_throwing_silk;
	}

	void throw_barbs();
	void throw_swords();

	void on_throw_silk();
	void on_dash();
private:
	bool is_throwing_silk = false;
	bool is_dash_in_air = false;
	bool is_dash_on_floor = false;

	Animation animation_silk;
	AnimationGroup animation_dash_in_air_vfx;
	AnimationGroup animation_dash_on_floor_vfx;
	Animation* current_animation_dash = nullptr;

	std::vector<Barb*> barb_list;
	std::vector<Sword*>sword_list;
	CollisionBox* collision_box_silk = nullptr;
};
