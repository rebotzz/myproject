#pragma once

// 角色状态节点

class Character;

class StateNode
{
protected:
	Character* parent_ = nullptr;
public:
	StateNode(Character* parent) :parent_(parent) {}
	StateNode(){}		// TODO：临时方案，代码重构完成后删除
	virtual ~StateNode() = default;

	virtual void on_enter() { };
	virtual void on_update(float delta) { };
	virtual void on_exit() { };
};

// 通用状态节点：闲置、奔跑、跳跃、下落、攻击
class StateNodeIdle : public StateNode
{
public:
	StateNodeIdle(Character* parent):StateNode(parent){}

	virtual void on_enter() override;
	virtual void on_update(float delta) override;
	virtual void on_exit() override;
};

//class StateNodeRun : public StateNode
//{
//public:
//	StateNodeRun(Character* parent) :StateNode(parent) {}
//
//	virtual void on_enter() override;
//	virtual void on_update(float delta) override;
//	virtual void on_exit() override;
//};
//
//class StateNodeJump : public StateNode
//{
//public:
//	StateNodeJump(Character* parent) :StateNode(parent) {}
//
//	virtual void on_enter() override;
//	virtual void on_update(float delta) override;
//	virtual void on_exit() override;
//};
//
//class StateNodeFall : public StateNode
//{
//public:
//	StateNodeFall(Character* parent) :StateNode(parent) {}
//
//	virtual void on_enter() override;
//	virtual void on_update(float delta) override;
//	virtual void on_exit() override;
//};
//
//class StateNodeAttack : public StateNode
//{
//public:
//	StateNodeAttack(Character* parent) :StateNode(parent) {}
//
//	virtual void on_enter() override;
//	virtual void on_update(float delta) override;
//	virtual void on_exit() override;
//};
