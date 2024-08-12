#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "platform.h"
#include "player.h"									// debug:忘记include对应头文件会出现奇怪的报错
#include "status_bar.h"
#include "buff_bullet.h"

extern bool is_debug;
extern SceneManager scene_manager;					
extern Camera main_camera;							// 摄像机
extern std::shared_ptr<Player> player_1;			// 玩家1
extern std::shared_ptr<Player> player_2;			// 玩家2
extern IMAGE* img_player_1_avatar;					// 玩家1头像
extern IMAGE* img_player_2_avatar;					// 玩家2头像
extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;
extern std::vector<Buff*> buff_list;


// 游戏背景
extern IMAGE img_sky;								// 背景天空
extern IMAGE img_hills;								// 背景山脉
extern IMAGE img_large_platform;					// 玩家站立大平台
extern IMAGE img_small_platform;					// 玩家站立小平台

// 玩家标识
extern IMAGE img_1P_cursor;							// 玩家1头顶光标
extern IMAGE img_2P_cursor;							// 玩家2头顶光标
extern IMAGE img_1P_winner;							// 玩家1获胜文本滚动条
extern IMAGE img_2P_winner;							// 玩家2获胜文本滚动条
extern IMAGE img_winner_bar;						// 获胜滚动条背景图

class GameScene : public Scene
{
private:
	POINT _pos_img_hills = { 0, 0 };
	POINT _pos_img_sky = { 0, 0 };
	StatusBar _status_bar_1P;						// 玩家1状态栏 
	StatusBar _status_bar_2P;						// 玩家2状态栏 
	bool _is_showing_cursor = false;				// 是否显示玩家头顶光标
	Timer _timer_showing_cursor;					// 显示玩家头顶光标定时器

	// 游戏结束部分
	bool _is_game_over = false;						// 游戏是否结束
	bool _is_slide_out_started = false;				// 是否开始滑动获胜文本
	POINT _pos_img_winner_bar = { 0, 0 };			// 玩家获胜滚动文本位置
	POINT _pos_img_winner_text = { 0, 0 };			// 获胜滚动条底色位置
	int _pos_x_img_winner_bar_dst = 0;				// 滚动条悬停位置
	int _pos_x_img_winner_text_dst = 0;				// 滚动文本悬停位置
	Timer _timer_winner_slide_in;					// 滚动条划入定时器
	Timer _timer_winner_slide_out;					// 滚动条划出定时器
	float _speed_winner_bar = 1.95f;				// 滚动条滑动速度
	float _speed_winner_text = 1.65f;				// 滚动文本滑动速度

	Timer _timer_random_buff;							// 场地随机buff定时器

public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter() override
	{
		_is_game_over = false;
		_is_slide_out_started = false;
		_is_showing_cursor = true;

		// 场景初始化
		_pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		_pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;
		_pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		_pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		platform_list.resize(4);
		Platform& large_platform = platform_list[0];
		large_platform._img = &img_large_platform;
		large_platform._render_position.x = 122;
		large_platform._render_position.y = 455;
		large_platform._shape.y = (float)large_platform._render_position.y + 60;
		// 碰撞检查边缘略小于图片
		large_platform._shape.left = (float)large_platform._render_position.x + 30;
		large_platform._shape.right = (float)large_platform._render_position.x + (float)large_platform._img->getwidth() - 30;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1._img = &img_small_platform;
		small_platform_1._render_position.x = 175;
		small_platform_1._render_position.y = 360;
		small_platform_1._shape.y = (float)small_platform_1._render_position.y + (float)small_platform_1._img->getheight() / 2;
		small_platform_1._shape.left = (float)small_platform_1._render_position.x + 40;
		small_platform_1._shape.right = (float)small_platform_1._render_position.x + (float)small_platform_1._img->getwidth() - 40;

		Platform& small_platform_2 = platform_list[2];
		small_platform_2._img = &img_small_platform;
		small_platform_2._render_position.x = 855;
		small_platform_2._render_position.y = 360;
		small_platform_2._shape.y = (float)small_platform_2._render_position.y + (float)small_platform_2._img->getheight() / 2;
		small_platform_2._shape.left = (float)small_platform_2._render_position.x + 40;
		small_platform_2._shape.right = (float)small_platform_2._render_position.x + (float)small_platform_2._img->getwidth() - 40;

		Platform& small_platform_3 = platform_list[3];
		small_platform_3._img = &img_small_platform;
		small_platform_3._render_position.x = 515;
		small_platform_3._render_position.y = 225;
		small_platform_3._shape.y = (float)small_platform_3._render_position.y + (float)small_platform_3._img->getheight() / 2;
		small_platform_3._shape.left = (float)small_platform_3._render_position.x + 40;
		small_platform_3._shape.right = (float)small_platform_3._render_position.x + (float)small_platform_3._img->getwidth() - 40;

		// 玩家位置,朝向初始化
		player_1->set_position({ 200, 50 });
		player_2->set_position({ 900, 50 });

		// 状态栏初始化
		_status_bar_1P.set_avatar(img_player_1_avatar);
		_status_bar_2P.set_avatar(img_player_2_avatar);
		_status_bar_1P.set_position(200, getheight() - 100);
		_status_bar_2P.set_position(750, getheight() - 100);

		// 玩家获胜滚动效果初始化
		_pos_img_winner_bar.x = 0 - img_winner_bar.getwidth();
		_pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		_pos_img_winner_text.x = 0 - img_1P_winner.getwidth();
		_pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		_pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;
		_pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;
		// 获胜滚动效果滑入持续2.5秒,之后滑出
		_timer_winner_slide_in.set_wait_time(2500);
		_timer_winner_slide_in.set_one_shot(true);
		_timer_winner_slide_in.set_callback([&]() {
			_is_slide_out_started = true;
			});
		// 获胜滚动效果滑出持续1秒,之后场景跳转回菜单
		_timer_winner_slide_out.set_wait_time(1000);
		_timer_winner_slide_out.set_one_shot(true);
		_timer_winner_slide_out.set_callback([]() {
			scene_manager.switch_scene(SceneManager::SceneType::MENU);
			});
		_timer_winner_slide_in.restart();
		_timer_winner_slide_out.restart();

		// 显示玩家头顶1P,2P光标定时器初始化
		_timer_showing_cursor.set_wait_time(2000);
		_timer_showing_cursor.set_one_shot(true);
		_timer_showing_cursor.set_callback([&]() { _is_showing_cursor = false; });
		_timer_showing_cursor.restart();

		// 场地buff生成
		_timer_random_buff.set_wait_time(7000);
		_timer_random_buff.set_one_shot(false);
		_timer_random_buff.set_callback(
			[&]()
			{
				int count = rand() % 2 + 1;
				for (int i = 0; i < count; ++i)
				{
					Buff* buff = nullptr;
					switch (rand() % 4)
					{
					case 0:
						buff = new BuffRecoveryHP;
						break;
					case 1:
						buff = new BuffRecoveryMP;
						break;
					case 2:
						buff = new BuffInvisible;
						break;
					case 3:
						buff = new BuffHurry;
						break;
					}
					buff_list.push_back(buff);
				}
			}
		);

		mciSendString(L"play bgm_game repeat from 0", nullptr, 0, nullptr);
	}
	virtual void on_input(const ExMessage& msg) override
	{
		static const int VK_Q = 0x51;

		player_1->on_input(msg);
		player_2->on_input(msg);

		if (msg.message == WM_KEYUP)
		{
			switch (msg.vkcode)
			{
			case VK_Q:
				is_debug = !is_debug;
				break;
			}
		}
	}

	virtual void on_update(int interval_ms) override
	{
		// 更新玩家
		player_1->on_update(interval_ms);
		player_2->on_update(interval_ms);

		// 更新摄像机
		main_camera.on_update(interval_ms);

		// 更新场地buff
		_timer_random_buff.on_update(interval_ms);

		// 移除可以删除的子弹: remove_if将判断条件为真的元素放在末尾,并返回新的末尾
		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(),
			[](Bullet* bullet)
			{
				bool deletable = bullet->check_can_remove();
				if (deletable) delete bullet;
				return deletable;
			}),
			bullet_list.end());
		// 移除失效buff
		buff_list.erase(std::remove_if(buff_list.begin(), buff_list.end(),
			[](Bullet* buff)
			{
				bool deletable = buff->check_can_remove();
				if (deletable) delete buff;
				return deletable;
			}),
			buff_list.end());

		// 更新子弹状态
		for (Bullet* bullet : bullet_list)
			bullet->on_update(interval_ms);

		// 更新场地随机buff状态
		for (Bullet* buff : buff_list)
			buff->on_update(interval_ms);

		// 更新状态栏
		_status_bar_1P.set_hp(player_1->get_hp());
		_status_bar_1P.set_mp(player_1->get_mp());
		_status_bar_1P.set_buff(player_1->get_buff());
		_status_bar_2P.set_hp(player_2->get_hp());
		_status_bar_2P.set_mp(player_2->get_mp());
		_status_bar_2P.set_buff(player_2->get_buff());

		// 更新显示玩家1P,2P光标倒计时
		if (_is_showing_cursor)
			_timer_showing_cursor.on_update(interval_ms);

		// 检测玩家是否坠落屏幕外
		Vector2 position_1P = player_1->get_position();
		Vector2 position_2P = player_2->get_position();
		if (position_1P.y > getheight())
			player_1->set_hp(0);
		if (position_2P.y > getheight())
			player_2->set_hp(0);

		// 游戏结束检测
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0)
		{
			if (!_is_game_over)
				mciSendString(L"play ui_win from 0", nullptr, 0, nullptr);

			_is_game_over = true;
		}

		if (_is_game_over)
		{
			_pos_img_winner_bar.x += (int)(_speed_winner_bar * interval_ms);
			_pos_img_winner_text.x += (int)(_speed_winner_text * interval_ms);

			// 在获胜滚动效果开始滑出之前,winner最多滑动到屏幕中间
			if (!_is_slide_out_started)
			{
				_timer_winner_slide_in.on_update(interval_ms);
				if (_pos_img_winner_bar.x >= _pos_x_img_winner_bar_dst)
					_pos_img_winner_bar.x = _pos_x_img_winner_bar_dst;
				if (_pos_img_winner_text.x >= _pos_x_img_winner_text_dst)
					_pos_img_winner_text.x = _pos_x_img_winner_text_dst;
			}
			else
				_timer_winner_slide_out.on_update(interval_ms);
		}
	}

	virtual void on_draw(const Camera& camera) override
	{
		// 背景
		putimage_alpha(_pos_img_sky.x, _pos_img_sky.y, &img_sky);
		putimage_alpha(_pos_img_hills.x, _pos_img_hills.y, &img_hills);

		// 平台
		for (Platform& platform : platform_list)
			platform.on_draw(camera);

		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(10, 10, L"调试模式中, 按'Q'键关闭");
		}

		// 玩家
		player_1->on_draw(camera);
		player_2->on_draw(camera);

		// 玩家头顶1P,2P光标
		if (_is_showing_cursor)
		{
			Vector2 pos_1P_cursor, pos_2P_cursor;
			pos_1P_cursor.x = player_1->get_position().x + (player_1->get_size().x - img_1P_cursor.getwidth()) / 2;
			pos_1P_cursor.y = player_1->get_position().y + - img_1P_cursor.getheight();
			pos_2P_cursor.x = player_2->get_position().x + (player_2->get_size().x - img_2P_cursor.getwidth()) / 2;
			pos_2P_cursor.y = player_2->get_position().y + -img_1P_cursor.getheight();
			putimage_alpha(camera, (int)pos_1P_cursor.x, (int)pos_1P_cursor.y, &img_1P_cursor);
			putimage_alpha(camera, (int)pos_2P_cursor.x, (int)pos_2P_cursor.y, &img_2P_cursor);
		}

		// 子弹
		for (Bullet* bullet : bullet_list)
			bullet->on_draw(camera);

		// buff
		for (Bullet* buff : buff_list)
			buff->on_draw(camera);

		// 状态栏
		if (!_is_game_over)
		{
			_status_bar_1P.on_draw();
			_status_bar_2P.on_draw();
		}
		// 游戏结束玩家获胜滚动效果
		else
		{
			putimage_alpha(camera, _pos_img_winner_bar.x, _pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(camera, _pos_img_winner_text.x, _pos_img_winner_text.y, 
				player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}

	}

	virtual void on_exit()
	{
		is_debug = false;
		_is_game_over = false;
		// 玩家对象通过智能指针自动释放(引用计数为0时)

		for (Bullet* bullet : bullet_list)
			delete bullet;
		bullet_list.clear();

		for (Bullet* buff : buff_list)
			delete buff;
		buff_list.clear();

		mciSendString(L"stop bgm_game", nullptr, 0, nullptr);
	}
};