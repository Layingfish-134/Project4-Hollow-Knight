#pragma once
#include"charactor.h"
class Player : public Charactor
{
public:
	Player()=default;
	~Player()=default;

	void on_update(float delta) override;
	void on_render()override;
	void on_input(const ExMessage& msg) override;

	void on_hurt()override;
private:
	enum class AttackDir {
		Up,Down,Left,Right
	};
public:
	void set_rolling(bool flag)
	{
		is_rolling = flag;
	}
	bool get_rolling()const
	{
		return is_rolling;
	}
	bool can_roll()const
	{
		return (!is_rolling && is_roll_cd_comp && is_roll_keydown);
	}
	void set_attacking(bool flag)
	{
		is_attacking = flag;
	}
	bool get_attacking()const
	{
		return is_attacking;
	}
	bool can_roll()const
	{
		return (!is_attacking && is_attack_cd_comp && is_attack_keydown);
	}
	bool can_jump() const
	{
		return (is_on_floor() && is_jump_keydown);
	}
	int get_move_axis()const
	{
		return (int)(is_right_keydown - is_left_keydown);
	}
	AttackDir get_attack_dir()
	{
		return attack_dir;
	}
public:
	void on_jump();
	void on_land();
	void on_roll();
	void on_attack();
private:
	void update_attack_dir(int x, int y);
private:
	bool is_rolling = false;
	bool is_roll_cd_comp = false;
	Timer timer_roll_cd;

	bool is_attacking = false;
	bool is_attack_cd_comp = false;
	Timer timer_attack_cd;

	bool is_jump_keydown = false;
	bool is_roll_keydown = false;
	bool is_left_keydown = false;
	bool is_right_keydown = false;
	bool is_attack_keydown = false;

	Animation animation_slash_up;
	Animation animation_slash_down;
	Animation animation_slash_left;
	Animation animation_slash_right;
	AttackDir attack_dir = AttackDir::Right;
	Animation* current_slash_animation = nullptr;

	bool is_jump_vfx_visible = false;
	Animation animation_jump_vfx;

	bool is_land_vfx_visible = false;
	Animation animation_land_vfx;
private:
	const int CD_ROLL = 0.75f;
	const int CD_ATTACK = 0.5f;
	const int SPEED_RUN = 300.0f;
	const int SPEED_JUMP = 780.0f;
	const int SPEED_ROLL = 800.0f;
};