#define _CRT_SECURE_NO_WARNINGS 1
#include"enemy_state_nodes.h"
#include"enemy.h"
#include"charactormanager.h"

EnemyAimNode::EnemyAimNode()
{
	timer.set_oneshot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout(
		[&]()
		{
			Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
			enemy->set_gravity_enabled(true);
			enemy->switch_state("dash_in_air");
		}
	);
}

void EnemyAimNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("aim");

	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	enemy->set_gravity_enabled(false);
	enemy->set_velocity({ 0,0 });
	timer.reset();

}
void EnemyAimNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	timer.on_update(delta);

	if (enemy->get_hp() <= 0)
	{

		enemy->switch_state("dead");
	}
}


void EnemyDashinAirNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("dash_in_air");

	const Charactor* player = CharactorManager::instance()->get_player();
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();

	Vector2 pos_target = { player->get_position().x,player->get_floor_y() };
	enemy->set_velocity((pos_target - enemy->get_position()).normalize() * SPEED_DASH);
	enemy->set_dash_in_air(true);
	enemy->set_gravity_enabled(false);
	enemy->on_dash();

	play_audio(_T("enemy_dash"), false);
}
void EnemyDashinAirNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->is_on_floor())
		enemy->switch_state("idle");
}
void EnemyDashinAirNode::on_exit()
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	enemy->set_gravity_enabled(true);
	enemy->set_dash_in_air(false);

}

EnemyDashonFloorNode::EnemyDashonFloorNode()
{
	timer.set_oneshot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout(
		[&]()
		{
			Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
			enemy->set_dash_on_floor(false);
		}
	);
}

void EnemyDashonFloorNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("dash_on_floor");

	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	enemy->set_velocity({ enemy->get_facing_left() ? -SPEED_DASH : SPEED_DASH,0 });
	enemy->set_dash_on_floor(true);
	enemy->on_dash();
	timer.reset();
	play_audio(_T("enemy_dash"), false);

}
void EnemyDashonFloorNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	timer.on_update(delta);
	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (!enemy->get_dash_on_floor())
		enemy->switch_state("idle");
}

void EnemyDeadNode::on_enter()
{
	MessageBox(GetHWnd(), _T("很好"), _T("你成功了"), MB_OK);
	exit(0);

}

void EnemyFallNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("fall");
}

void EnemyFallNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->is_on_floor())
		enemy->switch_state("idle");
}

EnemyIdleNode::EnemyIdleNode()
{
	timer.set_oneshot(true);
	timer.set_on_timeout(
		[&]()
		{
			Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
            
			int rand_num = range_random(0, 100);
			if (enemy->get_hp() > 5)
			{
				if (rand_num <= 25)
				{
					if (!enemy->is_on_floor())
						enemy->switch_state("fall");
					else
						enemy->switch_state("jump");
				}
				else if (rand_num <= 50)
				{
					if (!enemy->is_on_floor())
						enemy->switch_state("fall");
					else
						enemy->switch_state("run");
				}
				else if (rand_num <= 80)
					enemy->switch_state("squat");
				else if (rand_num <= 90)
					enemy->switch_state("throw_silk");
				else
					enemy->switch_state("throw_sword");
			}
			else
			{
				if (rand_num <= 25)
				{
					if (!enemy->is_on_floor())
						enemy->switch_state("fall");
					else
						enemy->switch_state("jump");
				}
				else if (rand_num <= 60)
					enemy->switch_state("throw_sword");
				else if (rand_num <= 70)
					enemy->switch_state("throw_silk");
				else if (rand_num <= 90)
					enemy->switch_state("throw_barb");
				else
					enemy->switch_state("squat");
			}
		}
	);
}
void EnemyIdleNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("idle");


	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	enemy->set_velocity({ 0,0 });

	float wait_time = 0;
	if (enemy->get_hp() >= 5)
		wait_time = range_random(0, 2) * 0.25f;
	else
		wait_time = range_random(0, 1) * 0.25f;

	timer.set_wait_time(wait_time);
	timer.reset();
}
void EnemyIdleNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	timer.on_update(delta);

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (enemy->get_velocity().y > 0)
		enemy->switch_state("fall");

}
void EnemyIdleNode::on_exit()
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	enemy->set_facing_left((enemy->get_position().x > 
		CharactorManager::instance()->get_player()->get_position().x));

}

void EnemyJumpNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("jump");
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();

	enemy->set_velocity({ 0,-SPEED_JUMP });

}
void EnemyJumpNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
    
	if (enemy->get_hp() <= 0)
	{
		enemy->switch_state("dead");
	}
	else if (enemy->get_velocity().y > 0)
	{
		int ran_num = range_random(0, 100);
		if (enemy->get_hp() > 5)
		{
			if (ran_num <= 50)
				enemy->switch_state("aim");
			else if (ran_num <= 80)
				enemy->switch_state("fall");
			else
				enemy->switch_state("throw_silk");
		}
		else
		{
			if (ran_num <= 50)
				enemy->switch_state("throw_silk");
			else if (ran_num <= 80)
				enemy->switch_state("fall");
			else
				enemy->switch_state("aim");
		}
	}
}

void EnemyRunNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("run");

	play_audio(_T("enemy_run"), true);

}
void EnemyRunNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	const Charactor* player = CharactorManager::instance()->get_player();
	
	const Vector2& pos_enemy = enemy->get_position();
	const Vector2& pos_player = player->get_position();

	enemy->set_velocity({ (pos_enemy.x > pos_player.x) ? -SPEED_RUN : SPEED_RUN,0 });

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
	else if (abs(pos_enemy.x - pos_player.x) <= MIN_DIS)
	{
		int rand_num = range_random(0, 100);
		if (enemy->get_hp() > 5)
		{
			if (rand_num <= 75)
				enemy->switch_state("squat");
			else
				enemy->switch_state("throw_silk");
		}
		else
		{
			if (rand_num <= 25)
				enemy->switch_state("squat");
			else
				enemy->switch_state("throw_silk");
		}
		stop_audio(_T("enemy_run"));

	}
}
void EnemyRunNode::on_exit()
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	enemy->set_velocity({ 0,0 });
}

EnemySquatState::EnemySquatState()
{
	timer.set_oneshot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout(
		[&]()
		{
			Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
			enemy->switch_state("dash_on_floor");
		}
	);
}

void EnemySquatState::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("squat");

	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	enemy->set_facing_left((enemy->get_position().x >
		CharactorManager::instance()->get_player()->get_position().x));
	timer.reset();
}
void EnemySquatState::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();

	timer.on_update(delta);

	if (enemy->get_hp()<=0)
		enemy->switch_state("dead");
}

EnemyThrowBarbsNode::EnemyThrowBarbsNode()
{
	timer.set_oneshot(true);
	timer.set_wait_time(0.8f);
	timer.set_on_timeout(
		[&]()
		{
			Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
			enemy->throw_barbs();
			enemy->switch_state("idle");
		}
	);
}

void EnemyThrowBarbsNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("throw_barb");

	timer.reset();

	play_audio(_T("enemy_throw_barbs"), false);
}
void EnemyThrowBarbsNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
    
	timer.on_update(delta);

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
}

EnemyThrowSilkNode::EnemyThrowSilkNode()
{
	timer.set_oneshot(true);
	timer.set_wait_time(0.9f);
	timer.set_on_timeout(
		[&]()
		{
			Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
			enemy->set_gravity_enabled(true);
			enemy->set_throwing_silk(false);

			if (!enemy->is_on_floor() && enemy->get_hp() > 5 && range_random(0, 100) <= 25)
			{
				enemy->switch_state("aim");
			}
			else if(enemy->is_on_floor())
			{
				enemy->switch_state("fall");
			}
			else
			{
				enemy->switch_state("idle");
			}
		}
	);
}

void EnemyThrowSilkNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("throw_silk");


	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
	enemy->set_gravity_enabled(false);
	enemy->set_velocity({ 0,0 });
	enemy->set_throwing_silk(true);
	enemy->on_throw_silk();

	timer.reset();
	play_audio(_T("enemy_throw_silk"), false);
}
void EnemyThrowSilkNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
    
	timer.on_update(delta);

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
}


EnemyThrowSwordNode::EnemyThrowSwordNode()
{
	timer_throw.set_wait_time(0.65f);
	timer_throw.set_oneshot(true);
	timer_throw.set_on_timeout(
		[&]()
		{
			Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
			enemy->throw_swords();

			play_audio(_T("enemy_throw_sword"), false);
		}
	);

	timer_switch.set_wait_time(1.0f);
	timer_switch.set_oneshot(true);
	timer_switch.set_on_timeout(
		[&]()
		{
			Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();
            
			int ran_num = range_random(0, 100);
			if (enemy->get_hp() > 5)
			{
				if (ran_num <= 50)
				{
					enemy->switch_state("squat");
				}
				else if (ran_num <= 80)
					enemy->switch_state("jump");
				else
					enemy->switch_state("idle");

			}
			else
			{
				if (ran_num <= 50)
					enemy->switch_state("jump");
				else if (ran_num <= 80)
					enemy->switch_state("throw_silk");
				else
					enemy->switch_state("idle");
			}
		}
	);
}

void EnemyThrowSwordNode::on_enter()
{
	CharactorManager::instance()->get_enemy()->set_animation("throw_sword");

	timer_switch.reset();
	timer_throw.reset();

}
void EnemyThrowSwordNode::on_update(float delta)
{
	Enemy* enemy = (Enemy*)CharactorManager::instance()->get_enemy();


	timer_switch.on_update(delta);
	timer_throw.on_update(delta);

	if (enemy->get_hp() <= 0)
		enemy->switch_state("dead");
}
