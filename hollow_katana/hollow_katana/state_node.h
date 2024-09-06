#pragma once

// ½ÇÉ«×´Ì¬½Úµã
class StateNode
{
public:
	StateNode() = default;
	~StateNode() = default;

	virtual void on_enter() { };
	virtual void on_update(float delta) { };
	virtual void on_exit() { };
};