#pragma once
#include"animation.h"
#include"Vector2.h"
#include"statemachine.h"
#include"collisionbox.h"
#include"timer.h"

#include<vector>
#include<string>
#include<unordered_map>

class Charactor
{
public:
	Charactor()=default;
	~Charactor()=default;
protected:
	struct AnimationGroup
	{
		Animation left;
		Animation right;
	};
protected:
	const float FLOOR_Y = 620;//地板的竖直方向坐标
	const float GRAVITY = 980 * 2;//重力大小
protected:
	int hp = 10;//角色生命值
	Vector2 position;//角色逻辑位置_脚底
	Vector2 velocity;//角色速度
	float logic_height;//角色逻辑高度
	bool is_facing_left = true;//角色面向左边
	StateMachine stata_machine;//角色状态机
	bool enable_gravity = true;//启用重力模拟
	bool is_invulnerable = false;//角色无敌状态
	Timer timer_invulnerable_blink;//无敌闪烁计时器
	Timer timer_invulnerable_status;//无敌时间计时器
	bool is_bilnk_visible=true;//角色无敌可见
	CollisionBox* hit_box = nullptr;//攻击碰撞箱
	CollisionBox* hurt_box = nullptr;//受击碰撞箱
	AnimationGroup* current_animation=nullptr;
	std::unordered_map<std::string, AnimationGroup*> animation_pool;
};