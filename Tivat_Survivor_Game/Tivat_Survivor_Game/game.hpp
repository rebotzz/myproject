#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <easyx.h>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern std::atomic<bool> is_started_game;
extern std::atomic<bool> running;
extern std::atomic<bool> play_hitvoice_enemy;
extern std::atomic<bool> play_hurtvoice_player;

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
	std::vector<IMAGE> frame_list;

public:
	Atlas() = default;
	Atlas(LPCTSTR path, int num)
	{
		// ���ض���֡ͼƬ
		TCHAR buff[256] = { 0 };
		for (int i = 0; i < num; ++i) {
			_stprintf_s(buff, path, i);
			IMAGE frame;
			loadimage(&frame, buff);
			frame_list.push_back(frame);
		}
	}

	// ���Ҿ���ͼƬ,����زĸ���
	void mirrorAtlas(Atlas* mirror_atlas)
	{
		// ��ʼ��
		mirror_atlas->frame_list.resize(frame_list.size(), IMAGE());
		for (int i = 0; i < frame_list.size(); ++i)
		{
			IMAGE& img_mirror = mirror_atlas->frame_list[i];
			IMAGE& img_origin = frame_list[i];
			int width = img_origin.getwidth(), height = img_origin.getheight();
			img_mirror.Resize(width, height);
			//Resize(&img_mirror, width, height);
			DWORD* img_origin_buffer = GetImageBuffer(&img_origin);
			DWORD* img_mirror_buffer = GetImageBuffer(&img_mirror);

			// image���߼����������ص�(R,G,B)���ɵĶ�ά����,�ڴ�����һά����
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					img_mirror_buffer[y * width + x] = img_origin_buffer[y * width + width - 1 - x];
				}
			}
		}
	}

	// �����زĵİ�ɫ��Ӱ
	void sketchImage(Atlas* sketch_atlas)
	{
		// ��ʼ��
		sketch_atlas->frame_list.resize(frame_list.size(), IMAGE());
		for (int i = 0; i < frame_list.size(); ++i)
		{
			IMAGE& img_sketch = sketch_atlas->frame_list[i];
			IMAGE& img_origin = frame_list[i];
			int width = img_origin.getwidth(), height = img_origin.getheight();
			img_sketch.Resize(width, height);
			DWORD* color_buff_origin_img = GetImageBuffer(&img_origin);
			DWORD* color_buff_sketch_img = GetImageBuffer(&img_sketch);

			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					// �ж����ص���Ƿ�͸��,͸��ͨ��alphaֵ,0~255,0��ȫ͸��
					int idx = y * width + x;
					if ((color_buff_origin_img[idx] & 0xff000000) >> 24) {
						// ����RGB(255, 255, 255),��eaxyXʹ�õ�COLORREF�ṹ������BGR,��������ȫ��͸����͸��ͨ��
						color_buff_sketch_img[idx] = BGR(RGB(255, 255, 255)) | (0xff << 24);
					}
				}
			}
		}
	}

	~Atlas() = default;
};


class Animation
{
private:
	int timer = 0;					// ������ʱ��,��֡���޹�,��ʱ���й�
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
		putimage_alpha(x, y, &anim_atlas->frame_list[idx_frame]);
	}


	// ���Թ���: ����Ч��
	void renderFrozen(int x, int y)
	{
		// ��ʼ��
		static bool init = true;
		static IMAGE ice_img;
		if (init) {
			loadimage(&ice_img, L"resource/img/ice.png");
			init = false;
		}

		// ����߹���Ч
		static int frozen_timer = 0;		// �����ʱ��
		static int highlight_pos_y = 0;		// ɨ������ֱ����
		static int THICKNESS = 5;			// ɨ���߿��
		static int frame_timer = 0;			// ֡��ʱ��
		static int cur_idx_frame = 0;		// ��ǰ֡
		static bool is_frozen = true;		

		if (++frame_timer % 900 == 0) {
			frame_timer = 0;
			is_frozen = !is_frozen;
		}

		if (is_frozen) {
			// ������ǰ֡���ں�������,����,����ֹ֡ͣ�仯
			IMAGE cur_frame_img = anim_atlas->frame_list[cur_idx_frame];
			int width = cur_frame_img.getwidth();
			int height = cur_frame_img.getheight();

			// ����ɨ����λ��
			highlight_pos_y += 1;
			// ���¶����ʱ��,������ɨ����λ��
			if (++frozen_timer % 300 == 0) {
				frozen_timer = 0;
				highlight_pos_y = 0;
			}

			// ������ǰ֡ɫ�ʻ�����,����͸��������л��
			DWORD* color_buff_frame_img = GetImageBuffer(&cur_frame_img);
			DWORD* color_buff_ice_img = GetImageBuffer(&ice_img);
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					int idx = y * width + x;
					DWORD color_ice_img = color_buff_ice_img[idx];
					DWORD color_frame_img = color_buff_frame_img[idx];
					if ((color_frame_img & 0xff000000) >> 24) {
						static const float RATIO = 0.25f;		// �������
						static const float THRESHOLD = 0.81f;	// ������ֵ 
						// colorbuff���ڴ�洢˳����B G R, ���Խ��� R B
						BYTE r = BYTE(GetBValue(color_frame_img)) * RATIO + BYTE(GetBValue(color_ice_img)) * (1 - RATIO);
						BYTE g = BYTE(GetGValue(color_frame_img)) * RATIO + BYTE(GetGValue(color_ice_img)) * (1 - RATIO);
						BYTE b = BYTE(GetRValue(color_frame_img)) * RATIO + BYTE(GetRValue(color_ice_img)) * (1 - RATIO);

						// ���߹�ɨ���߷�Χ�����ȸ�����ֵ�����ص��Ϊ��ɫ(�߹�)
						// ��ͬ��ɫ�����ȵĹ���ռ�Ȳ�ͬ,�˴����Ⱦ��鹫ʽ
						if (y >= highlight_pos_y && y <= highlight_pos_y + THICKNESS
							&& ((r / 255.0f) * 0.2126f + (g / 255.0f) * 0.7152f + (b / 255.0f) * 0.0722f) > THRESHOLD) {
							color_buff_frame_img[idx] = (DWORD)BGR(RGB(255, 255, 255)) | ((DWORD)((BYTE)255) << 24);
							continue;
						}

						// ����͸��ͨ�������µ����ص�	255(��͸��)
						color_buff_frame_img[idx] = (DWORD)BGR(RGB(r, g, b)) | (DWORD)((BYTE)0xff << 24);
					}
				}
			}

			putimage_alpha(x, y, &cur_frame_img);
		}
		else
		{
			static int counter = 0;
			counter += 1000 / 144;
			if (counter >= interval_ms) {
				counter = 0;
				cur_idx_frame = (cur_idx_frame + 1) % anim_atlas->frame_list.size();
			}
			putimage_alpha(x, y, &anim_atlas->frame_list[cur_idx_frame]);
		}
	}

	// ͬ������������֡����,֡��ʱ��: ��ɫ��ͬ״̬�����л��ν�����
	void synchronization(const Animation* anim)
	{
		timer = anim->timer;
		idx_frame = anim->idx_frame;
	}

	const Atlas* getAtlas()
	{
		return anim_atlas;
	}

	~Animation() = default;
};

// ��ɫ����
class CharaterAttribute
{
public:
	// ��ɫ����״̬: ���״̬,Ѫ��,�޵�֡������
	int _SPEED;
	int _HP;
	int _no_death_frame;
public:
	CharaterAttribute(int speed = 2, int HP = 1, int no_death_frame = 100)
		:_SPEED(speed), _HP(HP), _no_death_frame(no_death_frame) {}
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
	IMAGE* _img_shadow;
	Animation* _anim_left = nullptr;
	Animation* _anim_right = nullptr;
	Animation* _anim_right_sketch = nullptr;
	Animation* _anim_left_sketch = nullptr;

	// ��ɫ����
	int _SPEED = 3;
	int _HP = 1;
	int _no_death_frame = 100;

	// ��ɫ״̬: ���״̬,Ѫ��,�޵�֡������
	bool _alive = true;
	int _invincible_frame_count = 0;
	bool _invicible_status = false;
	bool _frozen_status = false;

	POINT _position = { 500, 500 };
	bool _face_left = true;

public:
	Character(Atlas* atlas_left, Atlas* atlas_right, Atlas* atlas_left_sketch, Atlas* atlas_right_sketch, IMAGE* img_shadow)
		:_img_shadow(img_shadow)
	{
		_anim_left = new Animation(atlas_left, 45);
		_anim_right = new Animation(atlas_right, 45);
		_anim_left_sketch = new Animation(atlas_left_sketch, 45);
		_anim_right_sketch = new Animation(atlas_right_sketch, 45);
		_FRAME_HEIGHT = _anim_left->getAtlas()->frame_list[0].getheight();
		_FRAME_WIDTH = _anim_left->getAtlas()->frame_list[0].getwidth();
		_SHADOW_IMG_WIDTH = _img_shadow->getwidth();
		_SHADOW_IMG_HEIGHT_DELTA = _FRAME_HEIGHT * 0.3;
	}

	bool checkAlive()
	{
		return _alive;
	}

	void setStatus(int speed, int HP, bool invicible_status = false, bool frozen_status = false)
	{
		_SPEED = speed;
		_HP = HP;
		if (invicible_status) _invicible_status = invicible_status;
		if (frozen_status) _frozen_status = frozen_status;
	}

	void updateStatus()
	{
		static int count = 0;
		// _no_death_frame֡��û������,�����޵�֡
		if (_invicible_status == true && ++count % _no_death_frame == 0) {
			_invicible_status = false;
			count = 0;
		}
	}

	void kill()
	{
		_alive = false;
	}

	// ���û�������
	void setCharaterAttribute(const CharaterAttribute& char_attri)
	{
		_SPEED = char_attri._SPEED;
		_HP = char_attri._HP;
		_no_death_frame = char_attri._no_death_frame;
	}

	void hurt()
	{
		// ���˺�Ķ����޵�״̬:����ÿһ֡��������,�޵�֡
		if (_invicible_status == false)
			_invincible_frame_count = 0;

		if (_invincible_frame_count++ % _no_death_frame == 0) {
			--_HP;
			_invicible_status = true;
			play_hitvoice_enemy = true;
		}

		if (_HP == 0) {
			_alive = false;
		}
	}

	// ���ƽ�ɫ����: delta_time����֡���,��֡���޹�,��ʱ���й�
	void draw(int delta_time)
	{
		int pos_shadow_x = _position.x + (_FRAME_WIDTH / 2 - _SHADOW_IMG_WIDTH / 2);
		int pos_shadow_y = _position.y + _FRAME_HEIGHT - _SHADOW_IMG_HEIGHT_DELTA;
		putimage_alpha(pos_shadow_x, pos_shadow_y, _img_shadow);

		// ��������ʱ����
		static const int interval_ms = 600;
		static int timer = 0;
		timer += delta_time;
		static bool normal_flag = true;

		if (_face_left) {
			// �޵�״̬,���沥�Ű�ɫ��Ӱ��ԭ�ж���
			if (_invicible_status) {
				if (timer >= interval_ms) {
					timer = 0;
					normal_flag = !normal_flag;
				}
				if (normal_flag)
					_anim_left->play(_position.x, _position.y, delta_time);
				else
					_anim_left_sketch->play(_position.x, _position.y, delta_time);
			}
			else {
				_anim_left->play(_position.x, _position.y, delta_time);
			}
		}
		else {
			if (_invicible_status) {
				if (timer >= interval_ms) {
					timer = 0;
					normal_flag = !normal_flag;
				}
				if (normal_flag)
					_anim_right->play(_position.x, _position.y, delta_time);
				else
					_anim_right_sketch->play(_position.x, _position.y, delta_time);
			}
			else {
				_anim_right->play(_position.x, _position.y, delta_time);

			}
		}
	}

	void testRenderFrozen(bool is_frozen = true)
	{
		static int x = 100, y = 100;
		//static int counter = 0;
		//if (++counter % 30 == 0) x++;
		_anim_left->renderFrozen(x, y);
	}

	const POINT& getPosition() const { return _position; }
	int width() const { return _FRAME_WIDTH; }
	int height() const { return _FRAME_HEIGHT; }

	~Character()
	{
		delete _anim_left;
		delete _anim_right;
		delete _anim_left_sketch;
		delete _anim_right_sketch;
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
	Player(Atlas* atlas_left, Atlas* atlas_right, Atlas* atlas_left_sketch, Atlas* atlas_right_sketch, IMAGE* img_shadow)
		:Character(atlas_left, atlas_right, atlas_left_sketch, atlas_right_sketch, img_shadow)
	{
		_SPEED = 3;
		_HP = 5;
		_SHADOW_IMG_HEIGHT_DELTA = 8;
		_no_death_frame = 200;
	}

	// ������Ϣ
	void processEvent(const ExMessage& msg)
	{
		static const int VK_A = 0x41;
		static const int VK_W = 0x57;
		static const int VK_S = 0x53;
		static const int VK_D = 0x44;
		static const int VK_F = 0x46;

		if (msg.message == WM_KEYDOWN) {
			// win11���ⰴ����
			switch (msg.vkcode)
			{
			case VK_UP:	_is_move_up = true; break;
			case VK_DOWN: _is_move_down = true; break;
			case VK_LEFT: _is_move_left = true; break;
			case VK_RIGHT: _is_move_right = true; break;

			case VK_W: _is_move_up = true; break;
			case VK_S: _is_move_down = true; break;
			case VK_A: _is_move_left = true; break;
			case VK_D: _is_move_right = true; break;
			case VK_F:
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

			case VK_W: _is_move_up = false;  break;
			case VK_S: _is_move_down = false; break;
			case VK_A: _is_move_left = false; break;
			case VK_D: _is_move_right = false; break;
			}
		}
	}

	// �������״̬
	void reset()
	{
		_HP = 5;
		_MP = 1;
		_alive = true;
		_SPEED = 3;
		_bullet_count = 3;

		_position = { 500, 500 };
		_is_move_up = false;
		_is_move_down = false;
		_is_move_left = false;
		_is_move_right = false;
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
		if (++count % interval == 0)
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
	Enemy(Atlas* atlas_left, Atlas* atlas_right, Atlas* atlas_left_sketch, Atlas* atlas_right_sketch, IMAGE* img_shadow)
		:Character(atlas_left, atlas_right, atlas_left_sketch, atlas_right_sketch, img_shadow)
	{
		_SPEED = 2;
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
	STATUS _status = STATUS::IDLE;

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
			if (checkCursorPosition(msg.x, msg.y))
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
		return (x >= _region.left && x <= _region.right)
			&& (y >= _region.top && y <= _region.bottom);
	}
};

class CharactersAtlas
{
public:
	std::shared_ptr<Atlas> atlas_character_left = nullptr, atlas_character_right = nullptr;
	std::shared_ptr<Atlas> atlas_character_left_sketch = nullptr, atlas_character_right_sketch = nullptr;

public:
	CharactersAtlas() = default;

	CharactersAtlas(const std::wstring& path, int num)
	{
		loadCharacterAtlas(path, num, atlas_character_left, atlas_character_right, atlas_character_left_sketch, atlas_character_right_sketch);
	}

	~CharactersAtlas() = default;

	Atlas* get_left() { return atlas_character_left.get(); }
	Atlas* get_right() { return atlas_character_right.get(); }
	Atlas* get_left_sketch() { return atlas_character_left_sketch.get(); }
	Atlas* get_right_sketch() { return atlas_character_right_sketch.get(); }

	// �����ز���Դ
	static void loadCharacterAtlas(const std::wstring& path, int num, std::shared_ptr<Atlas>& atlas_character_left, std::shared_ptr<Atlas>& atlas_character_right, std::shared_ptr<Atlas>& atlas_character_left_sketch, std::shared_ptr<Atlas>& atlas_character_right_sketch)
	{
		atlas_character_left = std::shared_ptr<Atlas>(new Atlas(path.c_str(), num));
		atlas_character_right = std::shared_ptr<Atlas>(new Atlas);
		atlas_character_left->mirrorAtlas(atlas_character_right.get());

		atlas_character_left_sketch = std::shared_ptr<Atlas>(new Atlas);
		atlas_character_left->sketchImage(atlas_character_left_sketch.get());
		atlas_character_right_sketch = std::shared_ptr<Atlas>(new Atlas);
		atlas_character_right->sketchImage(atlas_character_right_sketch.get());
	}
};

class StartGameButton : public Button
{
public:
	StartGameButton(RECT region, LPCTSTR path_idle, LPCTSTR path_hovered, LPCTSTR path_pushed)
		:Button(region, path_idle, path_hovered, path_pushed) {}

	~StartGameButton() = default;

protected:
	virtual void onClick()
	{
		is_started_game = true;

		// �����߳�,���ⲥ�Ŵ����Ч����
		auto thread_routine_music = []()
			{
				// �߳��ж���ջ֡,����main()�е���mciSendString���̲߳��ɼ�
				mciSendString(L"open resource/mus/bgm.mp3 alias bgm", nullptr, 0, nullptr);
				mciSendString(L"open resource/mus/hit.wav alias hit", nullptr, 0, nullptr);
				mciSendString(L"open resource/mus/hurt.wav alias hurt", nullptr, 0, nullptr);
				mciSendString(L"play bgm repeat from 0", nullptr, 0, nullptr);
				while (running)
				{
					if (is_started_game)
						mciSendString(L"resume bgm", nullptr, 0, nullptr);
					else
						mciSendString(L"stop bgm", nullptr, 0, nullptr);

					if (play_hitvoice_enemy) {
						mciSendString(L"play hit from 0", nullptr, 0, nullptr);
						play_hitvoice_enemy = false;
					}
					if (play_hurtvoice_player) {
						mciSendString(L"play hurt from 0", nullptr, 0, nullptr);
						play_hurtvoice_player = false;
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



void tryGenerateEnmey(std::vector<Enemy*>& enemy_list, Atlas* atlas_enemy_left, Atlas* atlas_enemy_right, Atlas* atlas_left_sketch, Atlas* atlas_right_sketch, IMAGE* img_enemy_shadow, const CharaterAttribute& enemy_attribute = CharaterAttribute())
{
	static int count = 0;
	static const int interval = 100;
	if (++count % interval == 0) {
		enemy_list.push_back(new Enemy(atlas_enemy_left, atlas_enemy_right, atlas_left_sketch, atlas_right_sketch, img_enemy_shadow));
		enemy_list.back()->setCharaterAttribute(enemy_attribute);
	}
}

void updateBullets(std::vector<Bullet>& bullet_list, const Player& player)
{
	// ��ʼ��
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






void loadButtonImage(std::shared_ptr<StartGameButton>& btn_start_game, std::shared_ptr<QuitGameButton>& btn_quit_game)
{
	static const int BUTTON_WIDTH = 192;
	static const int BUTTON_HEIGHT = 75;

	RECT region_btn_start_game, region_btn_quit_game;

	region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
	region_btn_start_game.top = 430;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;

	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 550;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;

	// �ƶ�����,forward����ת��, ���ʻ���new����
	btn_start_game = std::make_shared<StartGameButton>(region_btn_start_game, _T("resource/img/ui_start_idle.png"),
		_T("resource/img/ui_start_hovered.png"), _T("resource/img/ui_start_pushed.png"));
	btn_quit_game = std::make_shared<QuitGameButton>(region_btn_quit_game, _T("resource/img/ui_quit_idle.png"),
		_T("resource/img/ui_quit_hovered.png"), _T("resource/img/ui_quit_pushed.png"));
}