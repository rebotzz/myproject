
// 历史版本记录
#if 0

// v0.1 一击必杀版

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
	Player(Atlas* atlas_left, Atlas* atlas_right)
	{
		_anim_left = new Animation(atlas_left, 45);
		_anim_right = new Animation(atlas_right, 45);
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

	const POINT& getPosition() const
	{
		return _position;
	}

	int width() const { return _FRAME_WIDTH; }
	int height() const { return _FRAME_HEIGHT; }

	~Player()
	{
		delete _anim_left;
		delete _anim_right;
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

// 敌人类
class Enemy
{
private:
	const int _SPEED = 2;
	const int _FRAME_HEIGHT = 80;		// 敌人高度
	const int _FRAME_WIDTH = 80;		// 敌人宽度
	const int _SHADOW_IMG_WIDTH = 48;	// 敌人阴影宽度

private:
	// 动画帧素材,位置
	IMAGE _img_shadow;
	Animation* _anim_left = nullptr;
	Animation* _anim_right = nullptr;
	POINT _position = { 500, 500 };
	bool _face_left = true;
	bool _alive = true;

public:
	Enemy(Atlas* atlas_left, Atlas* atlas_right)
	{
		_anim_left = new Animation(atlas_left, 45);
		_anim_right = new Animation(atlas_right, 45);
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

	// 碰撞检测
	bool checkPlayerCollision(const Player& player)
	{
		// 如果玩家形心在敌人动画帧所在的矩形区域
		int player_x = player.getPosition().x + player.width() / 2;
		int player_y = player.getPosition().y + player.height() / 2;
		if (player_x >= _position.x && player_x <= _position.x + _FRAME_WIDTH
			&& player_y >= _position.y && player_y <= _position.y + _FRAME_HEIGHT)
			return true;

		return false;
	}

	bool checkBulletCollision(const Bullet& bullet)
	{
		// 如果子弹在敌人动画帧所在的矩形区域
		if (bullet._position.x >= _position.x && bullet._position.x <= _position.x + _FRAME_WIDTH
			&& bullet._position.y >= _position.y && bullet._position.y <= _position.y + _FRAME_HEIGHT)
			return true;

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

	bool checkAlive()
	{
		return _alive;
	}

	void hurt()
	{
		_alive = false;
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
	RECT _region;		// 按钮矩形区域位置和大小
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
	// 检测鼠标位置是否在按钮区域
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
		:Button(region, path_idle, path_hovered, path_pushed) {}

	~StartGameButton() = default;

protected:
	virtual void onClick()
	{
		is_started_game = true;

		// 避免播放打击音效卡顿
		auto thread_routine_music = []()
			{
				// 线程有独立栈帧,所以main()中调用mciSendString对线程不可见
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


void tryGenerateEnmey(std::vector<Enemy*>& enemy_list, Atlas* atlas_enemy_left, Atlas* atlas_enemy_right)
{
	static int count = 0;
	static const int interval = 100;
	if (++count % interval == 0) {
		enemy_list.push_back(new Enemy(atlas_enemy_left, atlas_enemy_right));
	}
}

void updateBullets(std::vector<Bullet>& bullet_list, const Player& player)
{
	// 初始化
	int pos_player_x = player.getPosition().x + player.width() / 2;
	int pos_player_y = player.getPosition().y + player.height() / 2;
	const static double interval = 2 * 3.1415926 / bullet_list.size();
	const static double RADIAL_SPEED = 25e-4;						// 子弹的径向波动速度
	const static double TANGENT_SPEED = 45e-4;						// 子弹的切向旋转速度

	double timeclock = GetTickCount();
	double radius = 100 + sin(timeclock * RADIAL_SPEED) * 35;		// 子弹到玩家半径
	for (int i = 0; i < bullet_list.size(); ++i) {
		double radians = timeclock * TANGENT_SPEED + interval * i;	// 子弹与玩家间角度
		bullet_list[i]._position.x = pos_player_x + int(cos(radians) * radius);
		bullet_list[i]._position.y = pos_player_y + int(sin(radians) * radius);
	}

}

void drawPlayerScore(int score)
{
	static TCHAR text[64] = { 0 };
	_stprintf_s(text, L"当前玩家得分: %d", score);

	setbkmode(TRANSPARENT);	// 文字背景透明
	settextcolor(RGB(124, 252, 0));
	outtextxy(20, 40, text);
}

#endif