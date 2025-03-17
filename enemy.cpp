#define _CRT_SECURE_NO_WARNINGS 1
#include"enemy.h"
#include"resourcesmanager.h"
#include"collisionmanager.h"
#include"charactormanager.h"
#include"enemy_state_nodes.h"

Enemy::Enemy()
{
	is_facing_left = true;
	position = {1050,2000};
	logic_height = 150;

	hit_box->set_size({ 50,80 });
	hurt_box->set_size({ 100,180 });

	hit_box->set_layer_src(CollisionLayer::None);
	hit_box->set_layer_dst(CollisionLayer::Player);

	hurt_box->set_layer_dst(CollisionLayer::None);
	hurt_box->set_layer_src(CollisionLayer::Enemy);

	hurt_box->set_on_collide(
		[&]()
		{
			decrease_hp();
		}
	);

	collision_box_silk = CollisionManager::instance()->create_collision_box();
	collision_box_silk->set_size({ 225,225 });
	collision_box_silk->set_layer_src(CollisionLayer::None);
	collision_box_silk->set_layer_dst(CollisionLayer::Player);
	collision_box_silk->set_enabled(false);

	{
		{
			AnimationGroup& animation_aim = animation_pool["aim"];

			Animation& animaion_aim_left = animation_aim.left;
			animaion_aim_left.set_interval(0.05f);
			animaion_aim_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_aim_left.set_loop(false);
			animaion_aim_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_aim_left"));

			Animation& animaion_aim_right = animation_aim.right;
			animaion_aim_right.set_interval(0.05f);
			animaion_aim_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_aim_right.set_loop(false);
			animaion_aim_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_aim_right"));

		}
		{
			AnimationGroup& animation_dash_in_air = animation_pool["dash_in_air"];

			Animation& animaion_dash_in_air_left = animation_dash_in_air.left;
			animaion_dash_in_air_left.set_interval(0.05f);
			animaion_dash_in_air_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_dash_in_air_left.set_loop(true);
			animaion_dash_in_air_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dash_in_air_left"));

			Animation& animaion_dash_in_air_right = animation_dash_in_air.right;
			animaion_dash_in_air_right.set_interval(0.05f);
			animaion_dash_in_air_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_dash_in_air_right.set_loop(true);
			animaion_dash_in_air_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dash_in_air_right"));
		}
		{
			AnimationGroup& animation_dash_on_floor = animation_pool["dash_on_floor"];

			Animation& animaion_dash_on_floor_left = animation_dash_on_floor.left;
			animaion_dash_on_floor_left.set_interval(0.05f);
			animaion_dash_on_floor_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_dash_on_floor_left.set_loop(true);
			animaion_dash_on_floor_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dash_on_floor_left"));

			Animation& animaion_dash_on_floor_right = animation_dash_on_floor.right;
			animaion_dash_on_floor_right.set_interval(0.05f);
			animaion_dash_on_floor_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_dash_on_floor_right.set_loop(true);
			animaion_dash_on_floor_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dash_on_floor_right"));
		}
		{
			AnimationGroup& animation_fall = animation_pool["fall"];

			Animation& animaion_fall_left = animation_fall.left;
			animaion_fall_left.set_interval(0.1f);
			animaion_fall_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_fall_left.set_loop(true);
			animaion_fall_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_fall_left"));

			Animation& animaion_fall_right = animation_fall.right;
			animaion_fall_right.set_interval(0.1f);
			animaion_fall_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_fall_right.set_loop(true);
			animaion_fall_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_fall_right"));
		}
		{
			AnimationGroup& animation_idle = animation_pool["idle"];

			Animation& animaion_idle_left = animation_idle.left;
			animaion_idle_left.set_interval(0.1f);
			animaion_idle_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_idle_left.set_loop(true);
			animaion_idle_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_idle_left"));

			Animation& animaion_idle_right = animation_idle.right;
			animaion_idle_right.set_interval(0.1f);
			animaion_idle_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_idle_right.set_loop(true);
			animaion_idle_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_idle_right"));
		}
		{
			AnimationGroup& animation_jump = animation_pool["jump"];

			Animation& animaion_jump_left = animation_jump.left;
			animaion_jump_left.set_interval(0.1f);
			animaion_jump_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_jump_left.set_loop(false);
			animaion_jump_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_jump_left"));

			Animation& animaion_jump_right = animation_jump.right;
			animaion_jump_right.set_interval(0.1f);
			animaion_jump_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_jump_right.set_loop(false);
			animaion_jump_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_jump_right"));
		}
		{
			AnimationGroup& animation_run = animation_pool["run"];

			Animation& animaion_run_left = animation_run.left;
			animaion_run_left.set_interval(0.05f);
			animaion_run_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_run_left.set_loop(true);
			animaion_run_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_run_left"));

			Animation& animaion_run_right = animation_run.right;
			animaion_run_right.set_interval(0.05f);
			animaion_run_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_run_right.set_loop(true);
			animaion_run_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_run_right"));
		}
		{
			AnimationGroup& animation_squat = animation_pool["squat"];

			Animation& animaion_squat_left = animation_squat.left;
			animaion_squat_left.set_interval(0.05f);
			animaion_squat_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_squat_left.set_loop(false);
			animaion_squat_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_squat_left"));

			Animation& animaion_squat_right = animation_squat.right;
			animaion_squat_right.set_interval(0.05f);
			animaion_squat_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_squat_right.set_loop(false);
			animaion_squat_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_squat_right"));
		}
		{
			AnimationGroup& animation_throw_barb = animation_pool["throw_barb"];

			Animation& animaion_throw_barb_left = animation_throw_barb.left;
			animaion_throw_barb_left.set_interval(0.1f);
			animaion_throw_barb_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_throw_barb_left.set_loop(false);
			animaion_throw_barb_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_barb_left"));

			Animation& animaion_throw_barb_right = animation_throw_barb.right;
			animaion_throw_barb_right.set_interval(0.1f);
			animaion_throw_barb_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_throw_barb_right.set_loop(false);
			animaion_throw_barb_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_barb_right"));
		}
		{
			AnimationGroup& animation_throw_silk = animation_pool["throw_silk"];

			Animation& animaion_throw_silk_left = animation_throw_silk.left;
			animaion_throw_silk_left.set_interval(0.1f);
			animaion_throw_silk_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_throw_silk_left.set_loop(true);
			animaion_throw_silk_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_silk_left"));

			Animation& animaion_throw_silk_right = animation_throw_silk.right;
			animaion_throw_silk_right.set_interval(0.1f);
			animaion_throw_silk_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_throw_silk_right.set_loop(true);
			animaion_throw_silk_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_silk_right"));
		}
		{
			AnimationGroup& animation_throw_sword = animation_pool["throw_sword"];

			Animation& animaion_throw_sword_left = animation_throw_sword.left;
			animaion_throw_sword_left.set_interval(0.05f);
			animaion_throw_sword_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_throw_sword_left.set_loop(false);
			animaion_throw_sword_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_sword_left"));

			Animation& animaion_throw_sword_right = animation_throw_sword.right;
			animaion_throw_sword_right.set_interval(0.05f);
			animaion_throw_sword_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
			animaion_throw_sword_right.set_loop(false);
			animaion_throw_sword_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_sword_right"));
		}
	}
	{
		animation_silk.set_interval(0.1f);
		animation_silk.set_loop(false);
		animation_silk.set_anchor_mode(Animation::AnchorMode::Centered);
		animation_silk.add_frame(ResourcesManager::instance()->find_atlas("silk"));

		Animation& animation_dash_in_air_left = animation_dash_in_air_vfx.left;
		animation_dash_in_air_left.set_interval(0.1f);
		animation_dash_in_air_left.set_loop(false);
		animation_dash_in_air_left.set_anchor_mode(Animation::AnchorMode::Centered);
		animation_dash_in_air_left.add_frame(ResourcesManager::instance()->find_atlas("enenmy_vfx_dash_in_air_left"));

		Animation& animation_dash_in_air_right = animation_dash_in_air_vfx.right;
		animation_dash_in_air_right.set_interval(0.1f);
		animation_dash_in_air_right.set_loop(false);
		animation_dash_in_air_right.set_anchor_mode(Animation::AnchorMode::Centered);
		animation_dash_in_air_right.add_frame(ResourcesManager::instance()->find_atlas("enenmy_vfx_dash_in_air_right"));

		Animation& animation_dash_on_floor_left = animation_dash_on_floor_vfx.left;
		animation_dash_on_floor_left.set_interval(0.1f);
		animation_dash_on_floor_left.set_loop(false);
		animation_dash_on_floor_left.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
		animation_dash_on_floor_left.add_frame(ResourcesManager::instance()->find_atlas("enenmy_vfx_dash_on_floor_left"));

		Animation& animation_dash_on_floor_right = animation_dash_on_floor_vfx.right;
		animation_dash_on_floor_right.set_interval(0.1f);
		animation_dash_on_floor_right.set_loop(false);
		animation_dash_on_floor_right.set_anchor_mode(Animation::AnchorMode::Bottomcentered);
		animation_dash_on_floor_right.add_frame(ResourcesManager::instance()->find_atlas("enenmy_vfx_dash_on_floor_right"));

	}
	{
		state_machine.register_state("aim", new EnemyAimNode);
		state_machine.register_state("dash_in_air", new EnemyDashinAirNode);
		state_machine.register_state("dash_on_floor", new EnemyDashonFloorNode);
		state_machine.register_state("fall", new EnemyFallNode);
		state_machine.register_state("idle", new EnemyIdleNode);
		state_machine.register_state("jump", new EnemyJumpNode);
		state_machine.register_state("dead", new EnemyDeadNode);
		state_machine.register_state("run", new EnemyRunNode);
		state_machine.register_state("squat", new EnemySquatState);
		state_machine.register_state("throw_silk", new EnemyThrowSilkNode);
		state_machine.register_state("throw_barb", new EnemyThrowBarbsNode);
		state_machine.register_state("throw_sword", new EnemyThrowSwordNode);

		state_machine.on_entry("idle"); 
	}
}
Enemy::~Enemy()
{
	CollisionManager::instance()->destory_collision_box(collision_box_silk);
}

void Enemy::on_update(float delta)
{
	if (velocity.x >= 0.0001f)
		is_facing_left = (velocity.x < 0);
	Charactor::on_update(delta);

	hit_box->set_position(get_logic_center());

	if (is_throwing_silk)
	{
		collision_box_silk->set_position(get_logic_center());
		animation_silk.set_position(get_logic_center());
		animation_silk.on_update(delta);
	}
	if (is_dash_in_air || is_dash_on_floor)
	{
		current_animation_dash->set_position(is_dash_in_air ? get_logic_center() : position);
		current_animation_dash->on_update(delta);
	}

	for (Barb* barb : barb_list)
	{
		barb->on_update(delta);
	}

	for (Sword* sword : sword_list)
	{
		sword->on_update(delta);
	}

	barb_list.erase(std::remove_if(
		barb_list.begin(), barb_list.end(),
		[](Barb* barb)
		{
			bool can_remove = !barb->check_vaild();
			if (can_remove) delete barb;
			return can_remove;
		}), barb_list.end()
			);
	sword_list.erase(std::remove_if(
		sword_list.begin(), sword_list.end(),
		[](Sword* sword)
		{
			bool can_remove = !sword->check_vaild();
			if (can_remove)delete sword;
			return can_remove;
		}), sword_list.end()
			);

}
void Enemy::on_render()
{
	for (Barb* barb : barb_list)
	{
		barb->on_render();
	}
	for (Sword* sword : sword_list)
	{
		sword->on_render();
	}

	Charactor::on_render();

	if (is_throwing_silk)
		animation_silk.on_render();
	if (is_dash_in_air || is_dash_on_floor)
		current_animation_dash->on_render();


}
void Enemy::on_hurt()
{
	switch (range_random(1,3))
	{
	case 1: 
		play_audio(_T("enemy_hurt_1"), false);
		break;
	case 2:
		play_audio(_T("enemy_hurt_2"), false);
		break;
	case 3:
		play_audio(_T("enemy_hurt_3"), false);
		break;
	default:
		break;
	}
}
void Enemy::throw_barbs()
{
	int num_new_barbs = range_random(3, 6);
	if (barb_list.size() >= 10) num_new_barbs = 1;

	int width_grid = getwidth() / num_new_barbs;

	for (int i = 0; i < num_new_barbs; i++)
	{
		Barb* barb = new Barb;
		float x = range_random(width_grid * i, width_grid * (i + 1));
		float y = range_random(250, 500);
		barb->set_position({ x,y });
		barb_list.push_back(barb);

	}
}
void Enemy::throw_swords()
{
	Sword* sword = new Sword(get_logic_center(),is_facing_left);
	sword_list.push_back(sword);
}
void Enemy::on_throw_silk()
{
	animation_silk.reset();
}
void Enemy::on_dash()
{
	if (is_dash_in_air)
		current_animation_dash = velocity.x < 0 ? &animation_dash_in_air_vfx.left : &animation_dash_in_air_vfx.right;
	else
		current_animation_dash = velocity.x < 0 ? &animation_dash_on_floor_vfx.left : &animation_dash_on_floor_vfx.right;
	current_animation_dash->reset();
}
