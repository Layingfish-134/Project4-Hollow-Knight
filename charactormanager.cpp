#define _CRT_SECURE_NO_WARNINGS 1
#include"charactormanager.h"
#include"player.h"
#include"bullet-timer-manager.h"


CharactorManager* CharactorManager::manager = nullptr;

CharactorManager* CharactorManager::instance()
{
	if (!manager)
		manager = new CharactorManager;
	return manager;
}
void CharactorManager::on_input(ExMessage& msg)
{
	player->on_input(msg);
	//
}
void CharactorManager::on_update(float delta)
{
	player->on_update(delta);
	//
}
void CharactorManager::on_render()
{
	BulletTimeManager::instance()->post_progress();
	player->on_render();
	//
}

CharactorManager::CharactorManager()
{
	player = new Player();
	//
}
CharactorManager::~CharactorManager()
{
	delete player;
	//delete enemy;
}