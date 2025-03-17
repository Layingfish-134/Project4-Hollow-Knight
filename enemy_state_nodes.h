#pragma once
#include"timer.h"
#include"statenode.h"

class EnemyAimNode : public StateNode
{
public:
	EnemyAimNode();
	~EnemyAimNode()=default;

	void on_enter() override;
	void on_update(float delta) override;
private:
	Timer timer;
};


class EnemyDashinAirNode : public StateNode
{
public:
	EnemyDashinAirNode()=default;
	~EnemyDashinAirNode()=default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;
private:
	const float SPEED_DASH = 1500.0f;
};



class EnemyDashonFloorNode : public StateNode
{
public:
	EnemyDashonFloorNode();
	~EnemyDashonFloorNode() = default;

	void on_enter() override;
	void on_update(float delta) override;
private:
	const float SPEED_DASH = 1000.0f;
	Timer timer;
};

class EnemyDeadNode : public StateNode
{
public:
	EnemyDeadNode()=default;
	~EnemyDeadNode()=default;

	void on_enter()override;
private:

};

class EnemyFallNode : public StateNode
{
public:
	EnemyFallNode() = default;
	~EnemyFallNode() = default;

	void on_enter()override;
	void on_update(float delta) override;
private:

};


class EnemyIdleNode : public StateNode
{
public:
	EnemyIdleNode();
	~EnemyIdleNode() = default;

	void on_enter()override;
	void on_update(float delta) override;
	void on_exit()override;
private:
	Timer timer;
};

class EnemyJumpNode : public StateNode
{
public:
	EnemyJumpNode() = default;
	~EnemyJumpNode() = default;

	void on_enter()override;
	void on_update(float delta) override;
private:
	const float SPEED_JUMP = 1000.0f;
};

class EnemyRunNode : public StateNode
{
public:
	EnemyRunNode() = default;
	~EnemyRunNode() = default;

	void on_enter()override;
	void on_update(float delta) override;
	void on_exit()override;
private:
	const float SPEED_RUN = 500.0f;
	const float MIN_DIS = 350.0f;

};

class EnemySquatState: public StateNode
{
public:
	EnemySquatState();
	~EnemySquatState() = default;

	void on_enter() override;
	void on_update(float delta)override;
private:
	Timer timer;
};



class EnemyThrowBarbsNode : public StateNode
{
public:
	EnemyThrowBarbsNode();
	~EnemyThrowBarbsNode()=default;

	void on_enter() override;
	void on_update(float delta) override;

private:
	Timer timer;
};


class EnemyThrowSilkNode : public StateNode
{
public:
	EnemyThrowSilkNode();
	~EnemyThrowSilkNode() = default;

	void on_enter() override;
	void on_update(float delta) override;

public:
	Timer timer;
};

class EnemyThrowSwordNode : public StateNode
{
public:
	EnemyThrowSwordNode();
	~EnemyThrowSwordNode() = default;

	void on_enter() override;
	void on_update(float delta) override;

public:
	Timer timer_throw;
	Timer timer_switch;
};
