#include "status_bar.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "player.h"

StatusBar::StatusBar()
{
	ui_heart = ResourcesManager::instance()->find_image("ui_heart");
}

void StatusBar::on_update(float delta)
{
	Player* player = dynamic_cast<Player*>(CharacterManager::instance()->get_player());
	hp = player->get_hp();
	current_mp_ratio = player->get_current_bullet_time() / player->get_total_bullet_time();
}

void StatusBar::on_render()
{
	// 子弹时间条渲染
	static int mp_bar_y_begin = 30;
	static int mp_bar_height = 30;
	static int mp_bar = 300;
	static int mp_bar_x_begin = (getwidth() - mp_bar) / 2;
	int current_mp_bar = (int)(mp_bar * current_mp_ratio);

	setfillcolor(RGB(5, 5, 5));
	solidroundrect(mp_bar_x_begin, mp_bar_y_begin, mp_bar_x_begin + mp_bar, mp_bar_y_begin + mp_bar_height, 8, 8);
	setfillcolor(RGB(67, 47, 47));
	solidroundrect(mp_bar_x_begin, mp_bar_y_begin, mp_bar_x_begin + mp_bar + 3 * 2, mp_bar_y_begin + mp_bar_height + 3 * 2, 8, 8);

	setfillcolor(RGB(135, 206, 250));
	solidroundrect(mp_bar_x_begin + 3, mp_bar_y_begin + 3, mp_bar_x_begin + current_mp_bar, mp_bar_y_begin + mp_bar_height, 8, 8);

	// 角色生命ui渲染
	static int ui_heart_y_begin = 30;
	static int ui_heart_gap = 60;
	static int ui_heart_x_begin = 30;
	for (int i = 0; i < hp; ++i)
	{
		Rect rect_dst;
		rect_dst.x = ui_heart_x_begin + i * ui_heart_gap, rect_dst.y = ui_heart_y_begin;
		rect_dst.w = ui_heart->getwidth(), rect_dst.h = ui_heart->getheight();
		putimage_alpha_ex(ui_heart, &rect_dst);
	}
}