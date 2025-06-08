#pragma once

class GameObject
{
protected:
	GameObject* parent_ = nullptr;
public:
	virtual void on_input(const ExMessage& msg) {}
	virtual void on_update(float delta) {}
	virtual void on_render() {}

	void set_parent(GameObject* parent) { parent_ = parent; }
	GameObject* get_parent() const { return parent_; }
};