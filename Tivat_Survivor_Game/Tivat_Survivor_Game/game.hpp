#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <easyx.h>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

// ʹ��ϵͳ��ʵ��͸��ͨ��ͼƬ����, easyX��putimage����ʵ��
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
	int timer = 0;			// ������ʱ��
	int idx_frame = 0;		// ����֡����
	int interval_ms = 0;	// ����֡���
	std::vector<IMAGE*> frame_list;
public:
	Animation(LPCTSTR path, int num, int interval) :interval_ms(interval)
	{
		// ���ض���֡ͼƬ
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
		// �����ʱ������֡���,������һ֡����
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

// �ӵ���
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
	const int _FRAME_HEIGHT = 80;		// ��Ҹ߶�
	const int _FRAME_WIDTH = 80;		// ��ҿ��
	const int _SHADOW_IMG_WIDTH = 32;	// �����Ӱ���

private:
	// ��Ҷ���֡
	IMAGE _img_shadow;
	Animation* _anim_left = nullptr;
	Animation* _anim_right = nullptr;

	// ����ƶ�
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

	// ������Ϣ
	void processEvent(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN) {
			// win11���ⰴ����
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
		// ��֤ÿ�������ٶ�һ��
		int dir_x = _is_move_right - _is_move_left;
		int dir_y = -(_is_move_up - _is_move_down);
		double dir_len = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (dir_len > 1e-6) {	// if (dir_len != 0)
			double normalized_x = dir_x / dir_len;
			double normalized_y = dir_y / dir_len;
			_position.x += int(normalized_x * _SPEED);
			_position.y += int(normalized_y * _SPEED);
		}

		// ���λ�ò��ܳ����߽�
		_position.x = max(0, _position.x);
		_position.x = min(WINDOW_WIDTH - _FRAME_WIDTH, _position.x);
		_position.y = max(0, _position.y);
		_position.y = min(WINDOW_HEIGHT - _FRAME_HEIGHT, _position.y);
	}

	// ������Ҷ���	delta_time����֡���
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
	const int _FRAME_HEIGHT = 80;		// ���˸߶�
	const int _FRAME_WIDTH = 80;		// ���˿��
	const int _SHADOW_IMG_WIDTH = 48;	// ������Ӱ���

private:
	// ��Ҷ���֡
	IMAGE _img_shadow;
	Animation* _anim_left = nullptr;
	Animation* _anim_right = nullptr;

	// ����ƶ�
	POINT _position = { 500, 500 };
	bool _face_left = true;

public:
	Enemy(int anim_num)
	{
		_anim_left = new Animation(L"resource/img/enemy_left_%d.png", anim_num, 45);
		_anim_right = new Animation(L"resource/img/enemy_right_%d.png", anim_num, 45);
		loadimage(&_img_shadow, L"resource/img/shadow_enemy.png");

		// �߽紦���ˢ�µ���λ��
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

	// ������Ϣ
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
		// ׷�����
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

	// ������Ҷ���	delta_time����֡���
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
