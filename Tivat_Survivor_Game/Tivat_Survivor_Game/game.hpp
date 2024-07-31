#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <easyx.h>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

// 使用系统库实现透明通道图片加载, easyX的putimage不能实现
#pragma comment(lib, "MSIMG32.LIB")
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(nullptr), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

class Animation
{
private:
	int timer = 0;			// 动画计时器
	int idx_frame = 0;		// 动画帧索引
	int interval_ms = 0;	// 动画帧间隔
	std::vector<IMAGE*> frame_list;
public:
	Animation(LPCTSTR path, int num, int interval) :interval_ms(interval)
	{
		// 加载动画帧图片
		TCHAR buff[256] = { 0 };
		for (int i = 0; i < num; ++i) {
			_stprintf_s(buff, path, i);
			IMAGE* frame = new IMAGE;
			loadimage(frame, buff);
			frame_list.push_back(frame);
		}
	}

	void play(int x, int y, int delta_time)
	{
		// 如果计时器到达帧间隔,播放下一帧动画
		timer += delta_time;
		if (timer >= interval_ms) {
			timer = 0;
			idx_frame = (idx_frame + 1) % frame_list.size();
		}
		putimage_alpha(x, y, frame_list[idx_frame]);
	}

	~Animation()
	{
		for (auto ptr : frame_list) {
			delete ptr;
		}
	}
};

// 子弹类
class Bullet
{
public:
	POINT _position = { 0, 0 };

private:
	const int RADIUS = 10;

public:
	Bullet() = default;
	~Bullet() = default;

	void draw() const
	{
		setlinecolor(RGB(255, 155, 50));
		setfillcolor(RGB(200, 75, 10));
		fillcircle(_position.x, _position.y, RADIUS);
	}

};

class Player
{
private:
	const int _SPEED = 3;
	const int _FRAME_HEIGHT = 80;		// 玩家高度
	const int _FRAME_WIDTH = 80;		// 玩家宽度
	const int _SHADOW_IMG_WIDTH = 32;	// 玩家阴影宽度

private:
	// 玩家动画帧
	IMAGE _img_shadow;
	Animation* _anim_left = nullptr;
	Animation* _anim_right = nullptr;

	// 玩家移动
	POINT _position = { 500, 500 };
	bool _is_move_up = false;
	bool _is_move_down = false;
	bool _is_move_left = false;
	bool _is_move_right = false;
	bool _face_left = true;

public:
	Player(int anim_num)
	{
		_anim_left = new Animation(L"resource/img/player_left_%d.png", anim_num, 45);
		_anim_right = new Animation(L"resource/img/player_right_%d.png", anim_num, 45);
		loadimage(&_img_shadow, L"resource/img/shadow_player.png");
	}

	// 处理消息
	void processEvent(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN) {
			// win11虚拟按键码
			switch (msg.vkcode)
			{
			case VK_UP:	_is_move_up = true; break;
			case VK_DOWN: _is_move_down = true; break;
			case VK_LEFT: _is_move_left = true; break;
			case VK_RIGHT: _is_move_right = true; break;
			}

		}
		else if (msg.message == WM_KEYUP) {
			switch (msg.vkcode)
			{
			case VK_UP:	_is_move_up = false; break;
			case VK_DOWN: _is_move_down = false; break;
			case VK_LEFT: _is_move_left = false; break;
			case VK_RIGHT: _is_move_right = false; break;
			}
		}
	}

	void move()
	{
		// 保证每个方向速度一致
		int dir_x = _is_move_right - _is_move_left;
		int dir_y = -(_is_move_up - _is_move_down);
		double dir_len = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (dir_len > 1e-6) {	// if (dir_len != 0)
			double normalized_x = dir_x / dir_len;
			double normalized_y = dir_y / dir_len;
			_position.x += int(normalized_x * _SPEED);
			_position.y += int(normalized_y * _SPEED);
		}

		// 玩家位置不能超出边界
		_position.x = max(0, _position.x);
		_position.x = min(WINDOW_WIDTH - _FRAME_WIDTH, _position.x);
		_position.y = max(0, _position.y);
		_position.y = min(WINDOW_HEIGHT - _FRAME_HEIGHT, _position.y);
	}

	// 绘制玩家动画	delta_time动画帧间隔
	void draw(int delta_time)
	{
		int pos_shadow_x = _position.x + (_FRAME_WIDTH / 2 - _SHADOW_IMG_WIDTH / 2);
		int pos_shadow_y = _position.y + _FRAME_HEIGHT - 8;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &_img_shadow);

		int dir_x = _is_move_right - _is_move_left;
		if (dir_x < 0)
			_face_left = true;
		else if (dir_x > 0)
			_face_left = false;

		if (_face_left)
			_anim_left->play(_position.x, _position.y, delta_time);
		else
			_anim_right->play(_position.x, _position.y, delta_time);
	}

	POINT getPosition() const
	{
		return _position;
	}

	~Player()
	{
		delete _anim_left;
		delete _anim_right;
	}
};

class Enemy
{
private:
	const int _SPEED = 3;
	const int _FRAME_HEIGHT = 80;		// 敌人高度
	const int _FRAME_WIDTH = 80;		// 敌人宽度
	const int _SHADOW_IMG_WIDTH = 48;	// 敌人阴影宽度

private:
	// 玩家动画帧
	IMAGE _img_shadow;
	Animation* _anim_left = nullptr;
	Animation* _anim_right = nullptr;

	// 玩家移动
	POINT _position = { 500, 500 };
	bool _face_left = true;

public:
	Enemy(int anim_num)
	{
		_anim_left = new Animation(L"resource/img/enemy_left_%d.png", anim_num, 45);
		_anim_right = new Animation(L"resource/img/enemy_right_%d.png", anim_num, 45);
		loadimage(&_img_shadow, L"resource/img/shadow_enemy.png");

		// 边界处随机刷新敌人位置
		enum SpawnEdge {
			UP = 0,
			DOWN,
			LEFT,
			RIGHT
		};

		SpawnEdge edge = SpawnEdge(rand() % 4);
		switch (edge) {
		case SpawnEdge::UP: 
			_position.x = rand() % WINDOW_WIDTH;
			_position.y = -_FRAME_HEIGHT;
			break;
		case SpawnEdge::DOWN:
			_position.x = rand() % WINDOW_WIDTH;
			_position.y = WINDOW_HEIGHT + _FRAME_HEIGHT;
			break;
		case SpawnEdge::LEFT:
			_position.x = -_FRAME_WIDTH;
			_position.y = rand() % WINDOW_HEIGHT;
			break;
		case SpawnEdge::RIGHT:
			_position.x = WINDOW_WIDTH + _FRAME_WIDTH;
			_position.y = rand() % WINDOW_HEIGHT;
			break;
		}
	}

	// 处理消息
	bool checkPlayer(const Player& player)
	{

		return false;
	}

	bool checkBullet(const Bullet& bullet)
	{

		return false;
	}

	void move(const Player& player)
	{
		// 追击玩家
		POINT pos_player = player.getPosition();
		int dir_x = pos_player.x - _position.x;
		int dir_y = pos_player.y - _position.y;
		double dir_len = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (dir_len > 1e-6) {	
			double normalized_x = dir_x / dir_len;
			double normalized_y = dir_y / dir_len;
			_position.x += int(normalized_x * _SPEED);
			_position.y += int(normalized_y * _SPEED);
		}

		if (dir_x < 0)
			_face_left = true;
		else if (dir_x > 0)
			_face_left = false;
	}

	// 绘制玩家动画	delta_time动画帧间隔
	void draw(int delta_time)
	{
		int pos_shadow_x = _position.x + (_FRAME_WIDTH / 2 - _SHADOW_IMG_WIDTH / 2);
		int pos_shadow_y = _position.y + _FRAME_HEIGHT - 25;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &_img_shadow);

		if (_face_left)
			_anim_left->play(_position.x, _position.y, delta_time);
		else
			_anim_right->play(_position.x, _position.y, delta_time);
	}

	~Enemy()
	{
		delete _anim_left;
		delete _anim_right;
	}
};


void tryGenerateEnmey(std::vector<Enemy*>& enemy_list)
{
	static int count = 0;
	static const int interval = 100;
	if (++count % interval == 0) {
		enemy_list.push_back(new Enemy(6));
	}
}
