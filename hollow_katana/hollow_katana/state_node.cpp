#include "state_node.h"
#include "character.h"
#include "audio_manager.h"
#include "player.h"

void StateNodeIdle::on_enter()
{
	parent_->set_animation("idle");
}
void StateNodeIdle::on_update(float delta)
{
	// 跳转状态
	if (parent_->get_hp() <= 0)
		parent_->switch_state("dead");
	else if (parent_->can_attack())
		parent_->switch_state("attack");
	else if (parent_->get_velocity().y > 0)
		parent_->switch_state("fall");
	else if (parent_->can_jump())
		parent_->switch_state("jump");
	else if (parent_->is_on_floor() && parent_->get_move_axis() != 0)
		parent_->switch_state("run");
	else if (parent_->can_skill_1())
		parent_->release_skill_1();
	else if (parent_->can_skill_2())
		parent_->release_skill_2();
}

void StateNodeRun::on_enter()
{
	parent_->set_animation("run");

	AudioManager::instance()->play_audio_ex(_T("player_run"), true);
}

void StateNodeRun::on_update(float delta)
{
	if (parent_->get_hp() <= 0)
		parent_->switch_state("dead");
	else if (parent_->can_attack())
		parent_->switch_state("attack");
	else if (parent_->can_skill_1())
		parent_->release_skill_1();
	else if (parent_->can_skill_2())
		parent_->release_skill_2();
	else if (parent_->can_jump())
		parent_->switch_state("jump");
	else if (parent_->get_move_axis() == 0)
		parent_->switch_state("idle");
}

void StateNodeRun::on_exit()
{
	AudioManager::instance()->stop_audio_ex(_T("player_run"));
}

void StateNodeJump::on_enter()
{
	parent_->set_animation("jump");
	parent_->on_jump();

	AudioManager::instance()->play_audio_ex(_T("player_jump"));
}

void StateNodeJump::on_update(float delta)
{
	if (parent_->get_hp() <= 0)
		parent_->switch_state("dead");
	else if (parent_->can_attack())
		parent_->switch_state("attack");
	else if (parent_->get_velocity().y > 0)
		parent_->switch_state("fall");
	else if (parent_->is_on_floor())		// 新增传送平台后
		parent_->switch_state("idle");
}

void StateNodeFall::on_enter()
{
	parent_->set_animation("fall");
}
void StateNodeFall::on_update(float delta)
{
	if (parent_->get_hp() <= 0)
		parent_->switch_state("dead");
	else if (parent_->can_attack())
		parent_->switch_state("attack");
	else if (parent_->is_on_floor())
	{
		parent_->on_land();
		parent_->switch_state("idle");
		AudioManager::instance()->play_audio_ex(_T("player_land"));
	}
}

