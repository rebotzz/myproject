#include "state_machine.h"


void StateMachine::set_entry(const std::string& id)
{
	current_state = state_pool[id];
}

void StateMachine::register_state(const std::string& id, StateNode* state_node)
{
	state_pool[id] = state_node;
}

void StateMachine::switch_to(const std::string& id)
{
	if (current_state) 
		current_state->on_exit();
	current_state = state_pool[id];
	if (current_state) 
		current_state->on_enter();
}

void StateMachine::on_update(float delta)
{
	if (!current_state) return;
	if (need_init)
	{
		current_state->on_enter();
		need_init = false;
	}

	current_state->on_update(delta);
}