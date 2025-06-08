#include "control.h"
#include "character.h"
#include "player.h"
#include "enemy_hornet.h"

void Control::on_input(const ExMessage& msg)
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
		case VK_J:
			parent_->set_attack_keydown(true);
			break;
		case VK_R:
			parent_->set_skill_1_keydown(true);
			break;
		case VK_K:
			parent_->set_skill_2_keydown(true);
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
		case VK_J:
			parent_->set_attack_keydown(false);
			break;
		case VK_R:
			parent_->set_skill_1_keydown(false);
			break;
		case VK_K:
			parent_->set_skill_2_keydown(false);
			break;
		}
		break;
	default:
		break;
	}
}

void PlayerControlAWSD::on_input(const ExMessage& msg)
{
	// Debug: 鼠标点击后,键盘按键松开判断会有遗漏, 解决：主循环peekmessage时if改为while,逐个拉取事件队列,不漏
	auto player = dynamic_cast<Player*>(parent_);
	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case VK_A:
			player->is_left_key_down = true;
			break;
		case VK_W:
			player->is_jump_key_down = true;
			break;
		case VK_S:
			player->is_dash_key_down = true;
			break;
		case VK_D:
			player->is_right_key_down = true;
			break;
		case VK_R:
			player->is_skill_1_key_down = true;
			break;
		case VK_J:
			player->is_attack_key_down = true;
			player->update_attack_dir();
			break;
		case VK_K:
			player->is_bullet_time_key_down = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case VK_A:
			player->is_left_key_down = false;
			break;
		case VK_W:
			player->is_jump_key_down = false;
			break;
		case VK_S:
			player->is_dash_key_down = false;
			break;
		case VK_D:
			player->is_right_key_down = false;
			break;
		case VK_J:
			player->is_attack_key_down = false;
			break;
		case VK_K:
			player->is_bullet_time_key_down = false;
			break;
		case VK_R:
			player->is_skill_1_key_down = false;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		player->is_attack_key_down = true;
		player->update_attack_dir(msg.x, msg.y);
		break;
	case WM_LBUTTONUP:
		player->is_attack_key_down = false;
		break;
		// 子弹时间键位2
	case WM_RBUTTONDOWN:
		if (player->current_bullet_time > 0)
			player->is_bullet_time_key_down = true;
		break;
	case WM_RBUTTONUP:
		player->is_bullet_time_key_down = false;
		break;
	case WM_MOUSEMOVE:
		player->pos_cursor.x = (float)(msg.x - player->img_crosshair->getwidth() / 2);
		player->pos_cursor.y = (float)(msg.y - player->img_crosshair->getheight() / 2);
		break;
	default:
		break;
	}
}

void PlayerControlArrow::on_input(const ExMessage& msg)
{
	auto player = dynamic_cast<Player*>(parent_);
	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case VK_LEFT:
			player->is_left_key_down = true;
			break;
		case VK_UP:
			player->is_jump_key_down = true;
			break;
		case VK_DOWN:
			player->is_dash_key_down = true;
			break;
		case VK_RIGHT:
			player->is_right_key_down = true;
			break;
		case VK_Z:
			player->is_attack_key_down = true;
			player->update_attack_dir();
			break;
		case VK_C:
			player->is_skill_1_key_down = true;
			break;
		case VK_X:
			player->is_bullet_time_key_down = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case VK_LEFT:
			player->is_left_key_down = false;
			break;
		case VK_UP:
			player->is_jump_key_down = false;
			break;
		case VK_DOWN:
			player->is_dash_key_down = false;
			break;
		case VK_RIGHT:
			player->is_right_key_down = false;
			break;
		case VK_Z:
			player->is_attack_key_down = false;
			break;
		case VK_X:
			player->is_bullet_time_key_down = false;
			break;
		case VK_C:
			player->is_skill_1_key_down = false;
			break;
		}
		break;
	}
}

void HornetControl::on_input(const ExMessage& msg)
{
	auto hornet = dynamic_cast<EnemyHornet*>(parent_);
	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case VK_A:
			hornet->is_left_key_down = true;
			break;
		case VK_W:
			hornet->is_jump_key_down = true;
			break;
		case VK_S:
			hornet->is_dash_key_down = true;
			break;
		case VK_D:
			hornet->is_right_key_down = true;
			break;
		case VK_R:
			hornet->is_skill_1_key_down = true;
			break;
		case VK_J:
			hornet->is_attack_key_down = true;
			break;
		case VK_K:
			hornet->is_skill_2_key_down = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case VK_A:
			hornet->is_left_key_down = false;
			break;
		case VK_W:
			hornet->is_jump_key_down = false;
			break;
		case VK_S:
			hornet->is_dash_key_down = false;
			break;
		case VK_D:
			hornet->is_right_key_down = false;
			break;
		case VK_R:
			hornet->is_skill_1_key_down = false;
			break;
		case VK_J:
			hornet->is_attack_key_down = false;
			break;
		case VK_K:
			hornet->is_skill_2_key_down = false;
			break;
		}
		break;
	default:
		break;
	}
}