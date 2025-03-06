#define _CRT_SECURE_NO_WARNINGS 1
#include"statemachine.h"

void StateMachine::on_update(float delta)
{
	if (!current_node)
		return;
	if (need_init)
	{
		current_node->on_enter();
		need_init = false;
	}
	current_node->on_update(delta);
}
void StateMachine::on_entry(const std::string& id)
{
	current_node = state_list[id];
}
void StateMachine::switch_to(const std::string& id)
{
	if (current_node)
		current_node->on_exit();
	current_node = state_list[id];
	if (current_node)
		current_node->on_enter();

}
void StateMachine::register_state(const std::string& id, StateNode* state_node)
{
	state_list[id] = state_node;
}
