#define _CRT_SECURE_NO_WARNINGS 1
#include"player.h"
#include"player_state_nodes.h"
#include"charactormanager.h"

PlayerAttackState::PlayerAttackState()
{
	timer.set_wait_time(0.3f);
	timer.set_oneshot(true);
	timer.set_callback(
		[&]()
		{
			Player* player = (Player*)CharactorManager::instance()->get_player();
			player->set_attacking(false);

		}
	);
}
void PlayerAttackState::on_enter()
{
	CharactorManager::instance()->get_player()->set_animation("attack");
	Player* player = (Player*)CharactorManager::instance()->get_player();
	player->set_attacking(true);
	player->get_hit_box()->set_enabled(true);
	update_hit_box_position();
	player->on_attack();
	timer.reset();
	switch (range_random(1,3))
	{
	case 1: play_audio(_T("player_attack_1"), false);
		break;
	case 2: play_audio(_T("player_attack_2"), false);
		break;
	case 3: play_audio(_T("player_attack_3"), false);
		break;
	default:
		break;
	}
}
void PlayerAttackState::on_update(float delta)
{
	timer.on_update(delta);
	update_hit_box_position();

	Player* player = (Player*)CharactorManager::instance()->get_player();
	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->get_attacking())
	{
		if (player->get_velocity().y > 0)
			player->switch_state("fall");
		else if (player->get_move_axis() == 0)
			player->switch_state("idle");
		else if (player->get_move_axis() != 0 && player->is_on_floor())
			player->switch_state("run");
	}
}
void PlayerAttackState::on_exit()
{
	Player* player = (Player*)CharactorManager::instance()->get_player();
	player->set_attacking(false);
	player->get_hit_box()->set_enabled(false);
}
void PlayerAttackState::update_hit_box_position()
{
	Player* player = (Player*)CharactorManager::instance()->get_player();
	CollisionBox* hit_box = player->get_hit_box();
	Vector2 pos_center = player->get_logic_center();
	Vector2 pos_hit_box;
	switch (player->get_attack_dir())
	{
	case Player::AttackDir::Up:
		pos_hit_box = { pos_center.x,pos_center.y - hit_box->get_size().y / 2 };
		break;
	case Player::AttackDir::Down:
		pos_hit_box = { pos_center.x,pos_center.y + hit_box->get_size().y / 2 };
		break;
	case Player::AttackDir::Left:
		pos_hit_box = { pos_center.x- hit_box->get_size().x / 2,pos_center.y };
		break;
	case Player::AttackDir::Right:
		pos_hit_box = { pos_center.x+hit_box->get_size().x / 2,pos_center.y };
		break;
	default:
		break;
	}
	hit_box->set_position(pos_hit_box);
}

PlayerDeadState::PlayerDeadState()
{
	timer.set_wait_time(2.0f);
	timer.set_oneshot(true);
	timer.set_callback([&]()
		{
			MessageBox(GetHWnd(), _T("OH!!!!"), _T("游戏失败"), MB_OK);
			exit(0);
		}
	);
}
void PlayerDeadState::on_enter()
{
	CharactorManager::instance()->get_player()->set_animation("dead");

	play_audio(_T("player_dead"), false);
}
void PlayerDeadState::on_update(float delta)
{
	timer.on_update(delta);
}

void PlayerFallState::on_enter()
{
	CharactorManager::instance()->get_player()->set_animation("fall");
}
void PlayerFallState::on_update(float delta)
{
	Player* player = (Player*)CharactorManager::instance()->get_player();
	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->is_on_floor())
	{
		player->switch_state("idle");

		player->on_land();
		play_audio(_T("player_land"), false);
	}
	else if (player->can_attack())
		player->switch_state("attack");
}
void PlayerIdleState::on_enter()
{
	CharactorManager::instance()->get_player()->set_animation("idle");
}

void PlayerIdleState::on_update(float delta)
{
	Player* player = (Player*)CharactorManager::instance()->get_player();

	
	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->can_attack())
		player->switch_state("attack");
	else if (player->get_velocity().y > 0)
		player->switch_state("fall");
	else if (player->can_jump())
		player->switch_state("jump");
	else if (player->can_roll())
		player->switch_state("roll");
	else if (player->is_on_floor() && player->get_move_axis() != 0)
		player->switch_state("run");
		
}

void PlayerJumpState::on_enter()
{
	CharactorManager::instance()->get_player()->set_animation("jump");

	Player* player = (Player*)CharactorManager::instance()->get_player();
	player->on_jump();

	play_audio(_T("player_jump"), false);
}

void PlayerJumpState::on_update(float delta)
{
	Player* player = (Player*)CharactorManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->get_velocity().y > 0)
		player->switch_state("fall");
	else if (player->can_attack())
		player->switch_state("attack");
}

PlayerRollState::PlayerRollState()
{
	timer.set_wait_time(0.35f);
	timer.set_oneshot(true);
	timer.set_callback(
		[&]()
		{
			Player* player = (Player*)CharactorManager::instance()->get_player();
			player->set_rolling(false);
		}
	);
}
void PlayerRollState::on_enter()
{
	CharactorManager::instance()->get_player()->set_animation("roll");
	timer.reset();
	Player* player = (Player*)CharactorManager::instance()->get_player();
	player->set_rolling(true);
	player->on_roll();
	player->get_hit_box()->set_enabled(false);

	play_audio(_T("player_roll"), false);
}
void PlayerRollState::on_update(float delta)
{
	timer.on_update(delta);
	Player* player = (Player*)CharactorManager::instance()->get_player();
	if (!player->get_rolling())
	{
		if (player->get_move_axis() != 0)
			player->switch_state("run");
		else if (player->can_jump())
			player->switch_state("jump");
		else
			player->switch_state("idle");
	}
}
void PlayerRollState::on_exit()
{
	CharactorManager::instance()->get_player()->get_hurt_box()->set_enabled(true);
}

void PlayerRunState::on_enter()
{
	CharactorManager::instance()->get_player()->set_animation("run");

	play_audio(_T("player_run"), true);
}
void PlayerRunState::on_update(float delta)
{
	Player* player = (Player*)CharactorManager::instance()->get_player();

	if (player->get_hp() <= 0)
	{
		player->switch_state("dead");
	}
	else if (player->get_move_axis() == 0)
		player->switch_state("idle");
	else if (player->can_jump())
		player->switch_state("jump");
	else if (player->can_attack())
		player->switch_state("attack");
	else if (player->can_roll())
		player->switch_state("roll");


}
void PlayerRunState::on_exit()
{
	stop_audio(_T("player_run"));
}