#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <easyx.h>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern bool is_started_game;
extern std::atomic<bool> running;
extern std::atomic<bool> play_hitvoice;

#pragma comment(lib, "WINmm.lib")	// mciSendString() ý����ƽӿ�
#pragma comment(lib, "MSIMG32.LIB")
// ʹ��ϵͳ��ʵ��͸��ͨ��ͼƬ����, easyX��putimage����ʵ��
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(nullptr), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

// ��Ԫģʽ,������Ϸ��Դ(ģ��,��ͼ)
class Atlas
{
public:
	std::vector<IMAGE*> frame_list;

public:
	Atlas(LPCTSTR path, int num)
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

	~Atlas()
	{
		for (auto& img_ptr : frame_list)
			delete img_ptr;
	}
};


class Animation
{
private:
	int timer = 0;					// ������ʱ��
	int idx_frame = 0;				// ����֡����
	int interval_ms = 0;			// ����֡���
	Atlas* anim_atlas = nullptr;	// ����ͼƬ��Դ
public:
	Animation(Atlas* atlas, int interval) 
		:anim_atlas(atlas), interval_ms(interval)
	{}

	void play(int x, int y, int delta_time)
	{
		// �����ʱ������֡���,������һ֡����
		timer += delta_time;
		if (timer >= interval_ms) {
			timer = 0;
			idx_frame = (idx_frame + 1) % anim_atlas->frame_list.size();
		}
		putimage_alpha(x, y, anim_atlas->frame_list[idx_frame]);
	}

	const Atlas* getAtlas()
	{
		return anim_atlas;
	}

	~Animation() = default;
};

// ��ɫ��:���,����
class Character
{
protected:
	int _FRAME_HEIGHT = 80;				// ��ɫ�߶�
	int _FRAME_WIDTH = 80;				// ��ɫ���
	int _SHADOW_IMG_WIDTH = 32;			// ��ɫ��Ӱ���
	int _SHADOW_IMG_HEIGHT_DELTA = 25;	// ��Ӱ���ɫ�߶����λ��

	// ��ɫ����֡�ز�
	IMAGE _img_shadow;
	Animation* _anim_left = nullptr;
	Animation* _anim_right = nullptr;

	// ��ɫ�ƶ�
	int _SPEED = 3;
	POINT _position = { 500, 500 };
	bool _face_left = true;

	// ��ɫ״̬: ���״̬,Ѫ��,�޵�֡������
	bool _alive = true;
	int _HP = 1;	
	int invincible_frame_count = 0;

public:
	Character(Atlas* atlas_left, Atlas* atlas_right, std::wstring img_shadow_path)
	{
		_anim_left = new Animation(atlas_left, 45);
		_anim_right = new Animation(atlas_right, 45);
		loadimage(&_img_shadow, img_shadow_path.c_str());
		_FRAME_HEIGHT = _anim_left->getAtlas()->frame_list[0]->getheight();
		_FRAME_WIDTH = _anim_left->getAtlas()->frame_list[0]->getwidth();
		_SHADOW_IMG_WIDTH = _img_shadow.getwidth();
	}

	bool checkAlive()
	{
		return _alive;
	}

	void updateStatus()
	{
		static const int interval = 200;
		static int count = 0;
		// interval֡��û������,�����޵�֡
		if(++count % interval == 0)	
			invincible_frame_count = 0;
	}

	void hurt(int no_death_frame = 30)
	{
		// ����ÿһ֡��������,�޵�֡
		if(invincible_frame_count++ % no_death_frame == 0)
			--_HP;

		if (_HP == 0) {
			_alive = false;
		}
	}

	// ���ƽ�ɫ����	delta_time����֡���
	void draw(int delta_time)
	{
		int pos_shadow_x = _position.x + (_FRAME_WIDTH / 2 - _SHADOW_IMG_WIDTH / 2);
		int pos_shadow_y = _position.y + _FRAME_HEIGHT - _SHADOW_IMG_HEIGHT_DELTA;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &_img_shadow);

		if (_face_left)
			_anim_left->play(_position.x, _position.y, delta_time);
		else
			_anim_right->play(_position.x, _position.y, delta_time);
	}

	const POINT& getPosition() const { return _position; }
	int width() const { return _FRAME_WIDTH; }
	int height() const { return _FRAME_HEIGHT; }

	~Character()
	{
		delete _anim_left;
		delete _anim_right;
	}
};

// �����
class Player : public Character
{
private:
	bool _is_move_up = false;
	bool _is_move_down = false;
	bool _is_move_left = false;
	bool _is_move_right = false;

	// ���״̬:����
	int _MP = 1;
	int _bullet_count = 3;

public:
	Player(Atlas* atlas_left, Atlas* atlas_right, std::wstring img_shadow_path)
		:Character(atlas_left, atlas_right, img_shadow_path) 
	{
		_SPEED = 3;
		_HP = 5;
		_SHADOW_IMG_HEIGHT_DELTA = 8;
	}

	// ������Ϣ
	void processEvent(const ExMessage& msg)
	{
		enum class VK_KEY
		{
			VK_A = 0x41,
			VK_W = 0x57,
			VK_S = 0x53,
			VK_D = 0x44,
			VK_F = 0x46
		};

		if (msg.message == WM_KEYDOWN) {
			// win11���ⰴ����
			switch (msg.vkcode)
			{
			case VK_UP:	_is_move_up = true; break;
			case VK_DOWN: _is_move_down = true; break;
			case VK_LEFT: _is_move_left = true; break;
			case VK_RIGHT: _is_move_right = true; break;

			case int(VK_KEY::VK_W) : _is_move_up = true; break;
			case int(VK_KEY::VK_S) : _is_move_down = true; break;
			case int(VK_KEY::VK_A) : _is_move_left = true; break;
			case int(VK_KEY::VK_D) : _is_move_right = true; break;
			case int(VK_KEY::VK_F) :
				if (_MP >= 3) {
					_MP -= 3;
					skill_2();
				}
				break;
			case VK_SPACE: 
				if (_MP >= 3) {
					_MP -= 3;
					skill_1();
				}
				break;
			}
		}
		else if (msg.message == WM_KEYUP) {
			switch (msg.vkcode)
			{
			case VK_UP:	_is_move_up = false; break;
			case VK_DOWN: _is_move_down = false; break;
			case VK_LEFT: _is_move_left = false; break;
			case VK_RIGHT: _is_move_right = false; break;

			case int(VK_KEY::VK_W) : _is_move_up = false; break;
			case int(VK_KEY::VK_S) : _is_move_down = false; break;
			case int(VK_KEY::VK_A) : _is_move_left = false; break;
			case int(VK_KEY::VK_D) : _is_move_right = false; break;
			}
		}
	}

	void drawStatusLine()
	{
		static const int RADIUS = 10;
		// ����Ѫ��
		settextcolor(RGB(255, 255, 255));
		setlinecolor(RGB(255, 165, 79));
		setfillcolor(RGB(238, 44, 44));
		outtextxy(10, 10, _T("HP:"));
		for (int i = 0; i < _HP; ++i) {
			fillcircle(50 + 35 * i, 20, RADIUS);
		}

		// ��������
		setlinecolor(RGB(72, 118, 255));
		setfillcolor(RGB(0, 191, 255));
		outtextxy(10, 40, _T("MP:"));
		for (int i = 0; i < _MP; ++i) {
			fillcircle(50 + 35 * i, 50, RADIUS);
		}
	}

	void skill_1() 
	{
		_bullet_count++;
	}

	void skill_2() 
	{
		_HP++;
	}

	void incrementMP()
	{
		static int count = 0;
		static const int interval = 5;
		if(++count % interval == 0)
			++_MP;
	}

	int bulletCount() const
	{
		return _bullet_count;
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

		// ��ɫλ�ò��ܳ����߽�
		_position.x = max(0, _position.x);
		_position.x = min(WINDOW_WIDTH - _FRAME_WIDTH, _position.x);
		_position.y = max(0, _position.y);
		_position.y = min(WINDOW_HEIGHT - _FRAME_HEIGHT, _position.y);

		if (dir_x < 0)
			_face_left = true;
		else if (dir_x > 0)
			_face_left = false;
	}

	~Player() = default;
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

// ������
class Enemy : public Character
{

public:
	Enemy(Atlas* atlas_left, Atlas* atlas_right, std::wstring shadow_path, int speed = 2)
		:Character(atlas_left, atlas_right, shadow_path)
	{
		_SPEED = speed;
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

	// ��ײ���
	bool checkPlayerCollision(const Player& player)
	{
		// �����������ڵ��˶���֡���ڵľ�������
		int player_x = player.getPosition().x + player.width() / 2;
		int player_y = player.getPosition().y + player.height() / 2;
		if (player_x >= _position.x && player_x <= _position.x + _FRAME_WIDTH
			&& player_y >= _position.y && player_y <= _position.y + _FRAME_HEIGHT)
			return true;

		return false;
	}

	bool checkBulletCollision(const Bullet& bullet)
	{
		// ����ӵ��ڵ��˶���֡���ڵľ�������
		if (bullet._position.x >= _position.x && bullet._position.x <= _position.x + _FRAME_WIDTH
			&& bullet._position.y >= _position.y && bullet._position.y <= _position.y + _FRAME_HEIGHT)
			return true;
		
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

	~Enemy() = default;
};

class Button
{
private:
	enum class STATUS
	{
		IDLE = 0,
		HOVERED,
		PUSHED
	};

private:
	RECT _region;		// ��ť��������λ�úʹ�С
	IMAGE _img_idle;
	IMAGE _img_hovered;
	IMAGE _img_pushed;
	STATUS _status;

public:
	Button(RECT region, LPCTSTR path_idle, LPCTSTR path_hovered, LPCTSTR path_pushed)
		:_region(region)
	{
		loadimage(&_img_idle, path_idle);
		loadimage(&_img_hovered, path_hovered);
		loadimage(&_img_pushed, path_pushed);
	}


	void processEvent(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			if (_status == STATUS::IDLE && checkCursorPosition(msg.x, msg.y))
				_status = STATUS::HOVERED;
			else if (_status == STATUS::HOVERED && !checkCursorPosition(msg.x, msg.y))
				_status = STATUS::IDLE;
			break;
		case WM_LBUTTONDOWN:
			if (checkCursorPosition(msg.x, msg.y) || _status == STATUS::HOVERED)
				_status = STATUS::PUSHED;
			break;
		case WM_LBUTTONUP:
			if (_status == STATUS::PUSHED)
			{
				onClick();
				_status = STATUS::IDLE;
			}
			break;
		}
	}

	void draw()
	{
		switch (_status)
		{
		case STATUS::IDLE:
			putimage(_region.left, _region.top, &_img_idle);
			break;
		case STATUS::HOVERED:
			putimage(_region.left, _region.top, &_img_hovered);
			break;
		case STATUS::PUSHED:
			putimage(_region.left, _region.top, &_img_pushed);
			break;
		}
	}

	~Button() = default;

protected:
	virtual void onClick() = 0;

private:
	// ������λ���Ƿ��ڰ�ť����
	bool checkCursorPosition(int x, int y)	
	{
		return x >= _region.left && x <= _region.left + _region.right
			&& y >= _region.top && y <= _region.top + _region.bottom;
	}
};



class StartGameButton : public Button
{
public:
	StartGameButton(RECT region, LPCTSTR path_idle, LPCTSTR path_hovered, LPCTSTR path_pushed)
		:Button(region, path_idle, path_hovered, path_pushed){}

	~StartGameButton() = default;

protected:
	virtual void onClick()
	{
		is_started_game = true;

		// ���ⲥ�Ŵ����Ч����
		auto thread_routine_music = []()
			{
				// �߳��ж���ջ֡,����main()�е���mciSendString���̲߳��ɼ�
				mciSendString(L"open resource/mus/bgm.mp3 alias bgm", nullptr, 0, nullptr);
				mciSendString(L"open resource/mus/hit.wav alias hit", nullptr, 0, nullptr);
				mciSendString(L"play bgm repeat from 0", nullptr, 0, nullptr);
				while (running)
				{
					if (play_hitvoice) {
						mciSendString(L"play hit from 0", nullptr, 0, nullptr);
						play_hitvoice = false;
					}
					Sleep(1000 / 144);
				}
			};
		std::thread thread_music(thread_routine_music);
		thread_music.detach();
	}
};


class QuitGameButton : public Button
{
public:
	QuitGameButton(RECT region, LPCTSTR path_idle, LPCTSTR path_hovered, LPCTSTR path_pushed)
		:Button(region, path_idle, path_hovered, path_pushed) {}

	~QuitGameButton() = default;

protected:
	virtual void onClick()
	{
		running = false;
	}
};


void tryGenerateEnmey(std::vector<Enemy*>& enemy_list, Atlas * atlas_enemy_left, Atlas* atlas_enemy_right)
{
	static int count = 0;
	static const int interval = 100;
	if (++count % interval == 0) {
		enemy_list.push_back(new Enemy(atlas_enemy_left, atlas_enemy_right, L"resource/img/shadow_enemy.png"));
	}
}

void updateBullets(std::vector<Bullet>& bullet_list, const Player& player)
{
	// ��ʼ��
	if (bullet_list.size() < player.bulletCount())
		bullet_list.resize(player.bulletCount());

	int pos_player_x = player.getPosition().x + player.width() / 2;
	int pos_player_y = player.getPosition().y + player.height() / 2;
	double interval = 2 * 3.1415926 / bullet_list.size();
	const static double RADIAL_SPEED = 25e-4;						// �ӵ��ľ��򲨶��ٶ�
	const static double TANGENT_SPEED = 45e-4;						// �ӵ���������ת�ٶ�


	double timeclock = GetTickCount();
	double radius = 100 + sin(timeclock * RADIAL_SPEED) * 35;		// �ӵ�����Ұ뾶
	for (int i = 0; i < bullet_list.size(); ++i) {
		double radians = timeclock * TANGENT_SPEED + interval * i;	// �ӵ�����Ҽ�Ƕ�
		bullet_list[i]._position.x = pos_player_x + int(cos(radians) * radius);
		bullet_list[i]._position.y = pos_player_y + int(sin(radians) * radius);
	}

}

void drawPlayerScore(int score)
{
	static TCHAR text[64] = { 0 };
	_stprintf_s(text, L"��ǰ��ҵ÷�: %d", score);

	setbkmode(TRANSPARENT);	// ���ֱ���͸��
	settextcolor(RGB(124, 252, 0));
	outtextxy(20, 70, text);
}