#include "state_node.h"
#include "character.h"


void StateNodeIdle::on_enter()
{
	parent_->set_animation("idle");
}
void StateNodeIdle::on_update(float delta)
{
	// Ìø×ª×´Ì¬
	if (parent_->get_hp() <= 0)
		parent_->switch_state("dead");
	else if (parent_->can_attack())
		parent_->switch_state("attack");
	else if (parent_->get_velocity().y > 0)
		parent_->switch_state("fall");
	else if (parent_->can_jump())
		parent_->switch_state("jump");
	else if (parent_->can_roll())
		parent_->switch_state("roll");
	else if (parent_->is_on_floor() && parent_->get_move_axis() != 0)
		parent_->switch_state("run");
	else if (parent_->can_dance())
		parent_->switch_state("dance");
}
void StateNodeIdle::on_exit()
{

}