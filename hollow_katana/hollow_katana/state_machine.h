#pragma once
#include <unordered_map>
#include <string>
#include "state_node.h"

class StateMachine
{
private:
	std::unordered_map<std::string, StateNode*> state_pool;
	bool need_init = true;
	StateNode* current_state = nullptr;

public:
	StateMachine() = default;
	~StateMachine();
	void set_entry(const std::string& id);
	void register_state(const std::string& id, StateNode* state_node);
	void switch_to(const std::string& id);
	void on_update(float delta);	
	void clear();
};

