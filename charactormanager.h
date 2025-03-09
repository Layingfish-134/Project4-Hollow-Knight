#pragma once
#include"charactor.h"

class CharactorManager
{
public:
	static CharactorManager* instance();

	Charactor* get_enemy()
	{
		return enemy;
	}

	Charactor* get_player()
	{
		return player;
	}
	
	void on_input(ExMessage& msg);
	void on_update(float delta);
	void on_render();

private:
	static CharactorManager* manager;

	Charactor* enemy=nullptr;
	Charactor* player=nullptr;
private:
	CharactorManager();
	~CharactorManager();

};
