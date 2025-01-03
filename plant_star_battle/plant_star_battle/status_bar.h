#pragma once
#include <easyx.h>
#include "util.h"
#include "buff_id.h"


extern IMAGE img_buff_icon_hurry;
extern IMAGE img_buff_icon_invisible;
extern IMAGE img_buff_icon_recover_hp;
extern IMAGE img_buff_icon_recover_mp;

// 角色状态栏
class StatusBar
{
private:
	int _hp = 0;						// 显示玩家生命值
	int _mp = 0;						// 显示玩家能量值
	IMAGE* _img_avator = nullptr;		// 显示玩家头像
	POINT _position = { 0 };			// 显示的位置
	const int _width = 275;				// 状态条宽度
	BuffID _buff_id = BuffID::INVALID;	// 玩家buff状态

public:
	void set_hp(int val)
	{
		_hp = val;
	}

	void set_mp(int val)
	{
		_mp = val;
	}

	void set_buff(BuffID id)
	{
		_buff_id = id;
	}

	void set_position(int x, int y)
	{
		_position.x = x;
		_position.y = y;
	}

	void set_avatar(IMAGE* img)
	{
		_img_avator = img;
	}

	void on_draw()
	{
		// 状态栏与摄相机位置无关
		putimage_alpha(_position.x, _position.y, _img_avator);

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(_position.x + 100, _position.y + 10, _position.x + 100 + _width + 3 * 2, _position.y + 36, 8, 8);
		solidroundrect(_position .x + 100, _position.y + 45, _position.x + 100 + _width + 3 * 2, _position.y + 71, 8, 8);
		setfillcolor(RGB(67, 47, 47));
		solidroundrect(_position.x + 100, _position.y + 10, _position.x + 100 + _width + 3, _position.y + 33, 8, 8);
		solidroundrect(_position.x + 100, _position.y + 45, _position.x + 100 + _width + 3, _position.y + 68, 8, 8);

		int hp_bar_width = (int)(max(0, _hp) / 100.0f * _width);
		int mp_bar_width = (int)(min(100, _mp) / 100.0f * _width);

		setfillcolor(RGB(197, 61, 67));
		solidroundrect(_position.x + 100, _position.y + 10, _position.x + 100 + hp_bar_width + 3, _position.y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(_position.x + 100, _position.y + 45, _position.x + 100 + mp_bar_width + 3, _position.y + 68, 8, 8);

		// buff icon绘制
		if (_buff_id == BuffID::HURRY)
			putimage_alpha(_position.x + 100, _position.y + 10 - img_buff_icon_hurry.getheight(), &img_buff_icon_hurry);
		else if (_buff_id == BuffID::INVISIBLE)
			putimage_alpha(_position.x + 100, _position.y + 10 - img_buff_icon_invisible.getheight(), &img_buff_icon_invisible);
		else if (_buff_id == BuffID::RECOVERY_HP)
			putimage_alpha(_position.x + 100, _position.y + 10 - img_buff_icon_recover_hp.getheight(), &img_buff_icon_recover_hp);
		else if (_buff_id == BuffID::RECOVERY_MP)
			putimage_alpha(_position.x + 100, _position.y + 10 - img_buff_icon_recover_mp.getheight(), &img_buff_icon_recover_mp);

	}

};