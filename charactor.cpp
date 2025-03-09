#define _CRT_SECURE_NO_WARNINGS 1
#include"charactor.h"
#include"collisionmanager.h"

Charactor::Charactor()
{
	hit_box = CollisionManager::instance()->create_collision_box();
	hurt_box = CollisionManager::instance()->create_collision_box();


	timer_invulnerable_status.set_wait_time(1.0f);
	timer_invulnerable_status.set_oneshot(true);
	timer_invulnerable_status.set_callback(
		[&]()
		{
			is_invulnerable = false;
		}
	);

	timer_invulnerable_blink.set_wait_time(0.075f);
	timer_invulnerable_blink.set_oneshot(false);
	timer_invulnerable_blink.set_callback(
		[&]()
		{
			is_bilnk_visible = !is_bilnk_visible;
		}
	);

}
Charactor::~Charactor()
{
	CollisionManager::instance()->destory_collision_box(hit_box);
	CollisionManager::instance()->destory_collision_box(hurt_box);
}
void Charactor::decrease_hp()
{
	if (is_invulnerable)
		return;
	hp--;
	if (hp > 0)
		make_invunerable();
	on_hurt();
}

void Charactor::on_update(float delta)
{
	state_machine.on_update(delta);

	//状态机更新
	if (hp <= 0)
		velocity.x = 0;
	if (enable_gravity)
		velocity.y += GRAVITY * delta;
	position += velocity * delta;

	if (position.y >= FLOOR_Y)
	{
		position.y = FLOOR_Y;
		velocity.y = 0;
	}
	if (position.x <= 0)
		position.x = 0;
	if (position.x >= getwidth())
		position.x = (float)getwidth();

	//位置更新

	hurt_box->set_position(get_logic_center());

	//碰撞箱跟随
	timer_invulnerable_status.on_update(delta);
	if (is_invulnerable)
		timer_invulnerable_blink.on_update(delta);

	if (!current_animation) return;

	Animation& animation = is_facing_left ? current_animation->left
		: current_animation->right;
	animation.on_update(delta);
	animation.set_position(position);
}

void Charactor::on_render()
{
	if (!current_animation || (is_invulnerable && !is_bilnk_visible))return;
	is_facing_left ? current_animation->left.on_render()
		: current_animation->right.on_render();

}

void Charactor::switch_state(const std::string& id)
{
	state_machine.switch_to(id);
}
void Charactor::set_animation(const std::string& id)
{
	current_animation = &animation_pool[id];
	current_animation->left.reset();
	current_animation->right.reset();
}


void Charactor::on_hurt(){}
void Charactor::on_input(const ExMessage& msg){}
