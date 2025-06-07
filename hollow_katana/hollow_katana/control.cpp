#include "control.h"

Control::~Control()
{
	if (move_control_)
	{
		delete move_control_;
	}
	if (skill_control_)
	{
		delete skill_control_;
	}
}

void Control::on_input(const ExMessage& msg)
{
	if (move_control_)
	{
		move_control_->on_input(msg);
	}
	if (skill_control_)
	{
		skill_control_->on_input(msg);
	}
}


void MoveControl::on_input(const ExMessage& msg)
{
	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case VK_A:
			parent_->set_left_keydown(true);
			break;
		case VK_W:
			parent_->set_jump_keydown(true);
			break;
		case VK_S:
			parent_->set_dash_keydown(true);
			break;
		case VK_D:
			parent_->set_right_keydown(true);
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case VK_A:
			parent_->set_left_keydown(false);
			break;
		case VK_W:
			parent_->set_jump_keydown(false);
			break;
		case VK_S:
			parent_->set_dash_keydown(false);
			break;
		case VK_D:
			parent_->set_right_keydown(false);
			break;
		}
		break;
	default:
		break;
	}
}