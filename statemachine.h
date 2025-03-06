#pragma once
#include"statenode.h"

#include<unordered_map>
#include<string>
class StateMachine
{
public:
	StateMachine()=default;
	~StateMachine()=default;
public:
	void on_update(float delta);

	void on_entry(const std::string& id);
	void switch_to(const std::string& id);
	void register_state(const std::string& id, StateNode* state_node);
private:
	bool need_init = true;
	StateNode* current_node=nullptr;
	std::unordered_map<std::string, StateNode*>state_list;
};