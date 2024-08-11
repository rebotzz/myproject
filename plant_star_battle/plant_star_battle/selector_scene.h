#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "animation.h"
#include "player_peashoter.h"
#include "player_sunflower.h"


extern SceneManager scene_manager;
extern std::shared_ptr<Player> player_1;			// ���1
extern std::shared_ptr<Player> player_2;			// ���2
extern IMAGE* img_player_1_avatar;						// ���1ͷ��
extern IMAGE* img_player_2_avatar;						// ���2ͷ��

// ��ɫͷ��ͼ��
extern IMAGE img_avatar_gloomshroom;
extern IMAGE img_avatar_nut;
extern IMAGE img_avatar_peashooter;
extern IMAGE img_avatar_sunflower;

// ��ɫѡ�����
extern IMAGE img_VS;
extern IMAGE img_selector_background;
extern IMAGE img_selector_tips;
extern IMAGE img_gravestone_right;
extern IMAGE img_gravestone_left;
extern IMAGE img_gloomshroom_selector_background_right;
extern IMAGE img_nut_selector_background_right;
extern IMAGE img_peashooter_selector_background_right;
extern IMAGE img_sunflower_selector_background_right;
extern IMAGE img_gloomshroom_selector_background_left;
extern IMAGE img_nut_selector_background_left;
extern IMAGE img_peashooter_selector_background_left;
extern IMAGE img_sunflower_selector_background_left;

extern IMAGE img_1P;
extern IMAGE img_1P_desc;
extern IMAGE img_1P_selector_btn_down_right;
extern IMAGE img_1P_selector_btn_idle_right;
extern IMAGE img_1P_selector_btn_down_left;
extern IMAGE img_1P_selector_btn_idle_left;
extern IMAGE img_2P;
extern IMAGE img_2P_desc;
extern IMAGE img_2P_selector_btn_down_right;
extern IMAGE img_2P_selector_btn_idle_right;
extern IMAGE img_2P_selector_btn_down_left;
extern IMAGE img_2P_selector_btn_idle_left;

// ��Ҷ���֡
extern Atlas atlas_player_gloomshroom_idle_right;
extern Atlas atlas_player_gloomshroom_idle_left;
extern Atlas atlas_player_nut_idle_right;
extern Atlas atlas_player_nut_idle_left;
extern Atlas atlas_player_peashooter_idle_right;
extern Atlas atlas_player_peashooter_idle_left;
extern Atlas atlas_player_sunflower_idle_right;
extern Atlas atlas_player_sunflower_idle_left;

// ��ɫͷ��ͼ��
extern IMAGE img_avatar_gloomshroom;
extern IMAGE img_avatar_nut;
extern IMAGE img_avatar_peashooter;
extern IMAGE img_avatar_sunflower;


class SelectorScene : public Scene
{
private:
	enum class PlayerType
	{
		PEASHOTER = 0,
		SUNFLOWER,
		NUT,
		GLOOMSHROOM,
		INVALID
	};

private:
	// ͼƬλ��
	POINT _pos_img_VS = { 0, 0 };									// vs����������
	POINT _pos_img_selector_background = { 0, 0 };					// ��������
	POINT _pos_img_selector_tips = { 0, 0 };						// ��ʼ��Ϸ������ʾ����
	POINT _pos_img_gravestone_1P = { 0, 0 };						// ���1P����Ĺ������
	POINT _pos_img_gravestone_2P = { 0, 0 };						// ���2P����Ĺ������
	POINT _pos_name_1P = { 0, 0 };									// ���1P��������
	POINT _pos_name_2P = { 0, 0 };									// ���2P��������

	POINT _pos_img_1P = { 0, 0 };									// ���1P��������
	POINT _pos_img_1P_desc = { 0, 0 };								// ���1P������������
	POINT _pos_img_1P_selector_btn_right = { 0, 0 };				// ���1P�Ҳ�ѡ�񰴼�����
	POINT _pos_img_1P_selector_btn_left = { 0, 0 };					// ���1P���ѡ�񰴼�����
	POINT _pos_img_2P = { 0, 0 };									// ���2P��������
	POINT _pos_img_2P_desc = { 0, 0 };								// ���2P������������
	POINT _pos_img_2P_selector_btn_right = { 0, 0 };				// ���2P�Ҳ�ѡ�񰴼�����
	POINT _pos_img_2P_selector_btn_left = { 0, 0 };					// ���2P���ѡ�񰴼�����
	POINT _pos_img_selector_background_1P = { 0, 0 };				// ���1P��������
	POINT _pos_img_selector_background_2P = { 0, 0 };				// ���2P��������
	POINT _pos_animation_1P = { 0, 0 };								// ���1P����λ��
	POINT _pos_animation_2P = { 0, 0 };								// ���2P����λ��

	bool _is_btn_1P_left_down = false;								// ���1P��ఴ���Ƿ���
	bool _is_btn_1P_right_down = false;								// ���1P�Ҳఴ���Ƿ���
	bool _is_btn_2P_left_down = false;								// ���2P��ఴ���Ƿ���
	bool _is_btn_2P_right_down = false;								// ���2P�Ҳఴ���Ƿ���

	// ��Ҳ��ݽ�ɫ������
	LPCTSTR _str_name_peashoter = L"�㶹����";
	LPCTSTR _str_name_sunflower = L"���տ�";
	LPCTSTR _str_name_nut = L"���";
	LPCTSTR _str_name_gloomshroom = L"����Ģ��";
	int _selector_background_scroll_offset_x = 0;					// ���������ƫ�ƾ���
	PlayerType _player_type_1 = PlayerType::PEASHOTER;				// ���1P��ɫ����
	PlayerType _player_type_2 = PlayerType::SUNFLOWER;				// ���2P��ɫ����
	Animation _animation_peashoter;									// �㶹���ֶ���
	Animation _animation_sunflower;									// ���տ�����
	Animation _animation_nut;										// �������
	Animation _animation_gloomshroom;								// ���տ�����

public:
	SelectorScene() = default;
	~SelectorScene() = default;


	virtual void on_enter()
	{
		// ��ʼ��ͼƬλ��	getwidth(),getheight()��ǰ��ͼ���ڳߴ�
		_pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;
		_pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;
		_pos_img_selector_background = { 0, 0 };
		_pos_img_selector_tips.x = (getwidth() - img_selector_tips.getwidth()) / 2;
		_pos_img_selector_tips.y = (getheight() - 125);
		const int OFFSET_X = 390;
		_pos_img_gravestone_1P.x = (getwidth() - img_gravestone_right.getwidth()) / 2 - OFFSET_X;
		_pos_img_gravestone_1P.y = (getheight() - img_gravestone_right.getheight()) / 2;
		_pos_img_gravestone_2P.x = (getwidth() - img_gravestone_left.getwidth()) / 2 + OFFSET_X;
		_pos_img_gravestone_2P.y = (getheight() - img_gravestone_left.getheight()) / 2;

		const int OFFSET_12P_Y = 50;
		_pos_img_1P.x = _pos_img_gravestone_1P.x + (img_gravestone_right.getwidth() - img_1P.getwidth()) / 2;
		_pos_img_1P.y = _pos_img_gravestone_1P.y - img_1P.getheight() - OFFSET_12P_Y;
		_pos_img_2P.x = _pos_img_gravestone_2P.x + (img_gravestone_right.getwidth() - img_2P.getwidth()) / 2;
		_pos_img_2P.y = _pos_img_gravestone_2P.y - img_2P.getheight() - OFFSET_12P_Y;
		_pos_img_1P_desc.x = _pos_img_gravestone_1P.x;
		_pos_img_1P_desc.y = getheight() - 125;
		_pos_img_2P_desc.x = _pos_img_gravestone_2P.x;
		_pos_img_2P_desc.y = getheight() - 125;

		const int OFFSET_BTN_X = 0;
		_pos_img_1P_selector_btn_left.x = _pos_img_gravestone_1P.x - img_1P_selector_btn_down_left.getwidth() - OFFSET_BTN_X;
		_pos_img_1P_selector_btn_left.y = _pos_img_gravestone_1P.y + (img_gravestone_right.getheight() - img_1P_selector_btn_down_left.getheight()) / 2;
		_pos_img_1P_selector_btn_right.x = _pos_img_gravestone_1P.x + img_gravestone_right.getwidth() + OFFSET_BTN_X;
		_pos_img_1P_selector_btn_right.y = _pos_img_gravestone_1P.y + (img_gravestone_right.getheight() - img_1P_selector_btn_down_right.getheight()) / 2;
		_pos_img_2P_selector_btn_left.x = _pos_img_gravestone_2P.x - img_2P_selector_btn_down_left.getwidth() - OFFSET_BTN_X;
		_pos_img_2P_selector_btn_left.y = _pos_img_gravestone_2P.y + (img_gravestone_right.getheight() - img_2P_selector_btn_down_left.getheight()) / 2;
		_pos_img_2P_selector_btn_right.x = _pos_img_gravestone_2P.x + img_gravestone_right.getwidth() + OFFSET_BTN_X;
		_pos_img_2P_selector_btn_right.y = _pos_img_gravestone_2P.y + (img_gravestone_right.getheight() - img_2P_selector_btn_down_right.getheight()) / 2;

		_pos_name_1P.x = _pos_img_gravestone_1P.x + 85;
		_pos_name_1P.y = _pos_img_gravestone_1P.y + 220;
		_pos_name_2P.x = _pos_img_gravestone_2P.x + 85;
		_pos_name_2P.y = _pos_img_gravestone_2P.y + 220;

		// ��Ҷ�����ʼ��
		_animation_peashoter.set_atlas(&atlas_player_peashooter_idle_right);
		_animation_peashoter.set_loop(true);
		_animation_peashoter.set_interval(100);
		_animation_sunflower.set_atlas(&atlas_player_sunflower_idle_left);
		_animation_sunflower.set_loop(true);
		_animation_sunflower.set_interval(100);
		_animation_nut.set_atlas(&atlas_player_nut_idle_left);
		_animation_nut.set_loop(true);
		_animation_nut.set_interval(100);
		_animation_gloomshroom.set_atlas(&atlas_player_gloomshroom_idle_left);
		_animation_gloomshroom.set_loop(true);
		_animation_gloomshroom.set_interval(100);

		_pos_animation_1P.x = _pos_img_gravestone_1P.x + 100;
		_pos_animation_1P.y = _pos_img_gravestone_1P.y + 90;
		_pos_animation_2P.x = _pos_img_gravestone_2P.x + img_gravestone_right.getwidth() - 100 - _animation_sunflower.get_frame()->getwidth();
		_pos_animation_2P.y = _pos_img_gravestone_2P.y + 90;

		mciSendString(L"play bgm_menu repeat from 0", nullptr, 0, nullptr);
	}

	// ������Ӱ���ı�����,���������
	void outtextxy_shaded(int x, int y, LPCTSTR str)
	{
		settextcolor(RGB(45, 45, 45));		// ��ɫ
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));	// ��ɫ
		outtextxy(x, y, str);
	}

	virtual void on_input(const ExMessage& msg)
	{
		static const int VK_A = 0x41;
		static const int VK_D = 0x44;

		if (msg.message == WM_KEYDOWN)
		{
			switch (msg.vkcode)
			{
			case VK_A:
				_is_btn_1P_left_down = true;
				break;
			case VK_D:
				_is_btn_1P_right_down = true;
				break;
			case VK_LEFT:
				_is_btn_2P_left_down = true;
				break;
			case VK_RIGHT:
				_is_btn_2P_right_down = true;
				break;
			default:
				break;
			}

		}

		if (msg.message == WM_KEYUP)
		{
			switch (msg.vkcode)
			{
			case VK_A:
				_is_btn_1P_left_down = false;
				_player_type_1 = (PlayerType)(((int)_player_type_1 - 1 + (int)PlayerType::INVALID) % (int)PlayerType::INVALID);
				mciSendString(L"play ui_switch from 0", nullptr, 0, nullptr);
				break;
			case VK_D:
				_is_btn_1P_right_down = false;
				_player_type_1 = (PlayerType)(((int)_player_type_1 + 1) % (int)PlayerType::INVALID);
				mciSendString(L"play ui_switch from 0", nullptr, 0, nullptr);
				break;
			case VK_LEFT:
				_is_btn_2P_left_down = false;
				_player_type_2 = (PlayerType)(((int)_player_type_2 - 1 + (int)PlayerType::INVALID) % (int)PlayerType::INVALID);
				mciSendString(L"play ui_switch from 0", nullptr, 0, nullptr);
				break;
			case VK_RIGHT:
				_is_btn_2P_right_down = false;
				_player_type_2 = (PlayerType)(((int)_player_type_2 + 1) % (int)PlayerType::INVALID);
				mciSendString(L"play ui_switch from 0", nullptr, 0, nullptr);
				break;

			case VK_RETURN:
				scene_manager.switch_scene(SceneManager::SceneType::GAME);
				mciSendString(L"play ui_confirm from 0", nullptr, 0, nullptr);
				break;
			default:
				break;
			}

		}

	}

	virtual void on_update(int interval_ms)
	{
		_animation_peashoter.on_update(interval_ms);
		_animation_sunflower.on_update(interval_ms);
		_animation_nut.on_update(interval_ms);
		_animation_gloomshroom.on_update(interval_ms);

		_selector_background_scroll_offset_x += 5;
		if (_selector_background_scroll_offset_x >= img_peashooter_selector_background_left.getwidth())
			_selector_background_scroll_offset_x = 0;
	}

	virtual void on_draw(const Camera& camera)
	{
		putimage(0, 0, &img_selector_background);
		// ָ��Է�����ͼƬ��ָ��
		IMAGE* img_background_1P_ptr = nullptr;
		IMAGE* img_background_2P_ptr = nullptr;

		switch (_player_type_1)
		{
		case PlayerType::PEASHOTER:
			img_background_2P_ptr = &img_peashooter_selector_background_left;
			break;
		case PlayerType::SUNFLOWER:
			img_background_2P_ptr = &img_sunflower_selector_background_left;
			break;
		case PlayerType::NUT:
			img_background_2P_ptr = &img_nut_selector_background_left;
			break;
		case PlayerType::GLOOMSHROOM:
			img_background_2P_ptr = &img_gloomshroom_selector_background_left;
			break;
		default:
			break;
		}

		switch (_player_type_2)
		{
		case PlayerType::PEASHOTER:
			img_background_1P_ptr = &img_peashooter_selector_background_right;
			break;
		case PlayerType::SUNFLOWER:
			img_background_1P_ptr = &img_sunflower_selector_background_right;
			break;
		case PlayerType::NUT:
			img_background_1P_ptr = &img_nut_selector_background_right;
			break;
		case PlayerType::GLOOMSHROOM:
			img_background_1P_ptr = &img_gloomshroom_selector_background_right;
			break;
		default:
			break;
		}

		// ʵ�ֹ�������Ч��: ���������������,���������ҹ���
		if (img_background_1P_ptr)
		{
			putimage_alpha(_selector_background_scroll_offset_x - img_background_1P_ptr->getwidth(), 0, img_background_1P_ptr);
			putimage_alpha(_selector_background_scroll_offset_x, 0,
				img_background_1P_ptr->getwidth() - _selector_background_scroll_offset_x, img_background_1P_ptr->getheight(),
				img_background_1P_ptr, 0, 0);
		}
		// �������������
		if (img_background_2P_ptr)
		{
			putimage_alpha(getwidth() - img_background_2P_ptr->getwidth(), 0,
				img_background_2P_ptr->getwidth() - _selector_background_scroll_offset_x, img_background_2P_ptr->getheight(),
				img_background_2P_ptr, _selector_background_scroll_offset_x, 0);
			putimage_alpha(getwidth() - _selector_background_scroll_offset_x, 0, img_background_2P_ptr);
		}

		putimage_alpha(_pos_img_gravestone_1P.x, _pos_img_gravestone_1P.y, &img_gravestone_right);
		putimage_alpha(_pos_img_gravestone_2P.x, _pos_img_gravestone_2P.y, &img_gravestone_left);
		putimage_alpha(_pos_img_1P.x, _pos_img_1P.y, &img_1P);
		putimage_alpha(_pos_img_2P.x, _pos_img_2P.y, &img_2P);
		putimage_alpha(_pos_img_1P_desc.x, _pos_img_1P_desc.y, &img_1P_desc);
		putimage_alpha(_pos_img_2P_desc.x, _pos_img_2P_desc.y, &img_2P_desc);

		putimage_alpha(_pos_img_1P_selector_btn_left.x, _pos_img_1P_selector_btn_left.y,
			_is_btn_1P_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
		putimage_alpha(_pos_img_1P_selector_btn_right.x, _pos_img_1P_selector_btn_right.y,
			_is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
		putimage_alpha(_pos_img_2P_selector_btn_left.x, _pos_img_2P_selector_btn_left.y,
			_is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
		putimage_alpha(_pos_img_2P_selector_btn_right.x, _pos_img_2P_selector_btn_right.y,
			_is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

		putimage_alpha(_pos_img_VS.x, _pos_img_VS.y, &img_VS);
		putimage_alpha(_pos_img_selector_tips.x, _pos_img_selector_tips.y, &img_selector_tips);

		switch (_player_type_1)
		{
		case PlayerType::PEASHOTER:
			_animation_peashoter.on_draw(camera, _pos_animation_1P.x, _pos_animation_1P.y);
			_pos_name_1P.x = _pos_img_gravestone_1P.x + (img_gravestone_left.getwidth() - textwidth(_str_name_peashoter)) / 2;
			outtextxy_shaded(_pos_name_1P.x, _pos_name_1P.y, _str_name_peashoter);
			break;
		case PlayerType::SUNFLOWER:
			_animation_sunflower.on_draw(camera, _pos_animation_1P.x, _pos_animation_1P.y);
			_pos_name_1P.x = _pos_img_gravestone_1P.x + (img_gravestone_left.getwidth() - textwidth(_str_name_sunflower)) / 2;
			outtextxy_shaded(_pos_name_1P.x, _pos_name_1P.y, _str_name_sunflower);
			break;
		case PlayerType::NUT:
			_animation_nut.on_draw(camera, _pos_animation_1P.x, _pos_animation_1P.y);
			_pos_name_1P.x = _pos_img_gravestone_1P.x + (img_gravestone_left.getwidth() - textwidth(_str_name_nut)) / 2;
			outtextxy_shaded(_pos_name_1P.x, _pos_name_1P.y, _str_name_nut);
			break;
		case PlayerType::GLOOMSHROOM:
			_animation_gloomshroom.on_draw(camera, _pos_animation_1P.x, _pos_animation_1P.y);
			_pos_name_1P.x = _pos_img_gravestone_1P.x + (img_gravestone_left.getwidth() - textwidth(_str_name_gloomshroom)) / 2;
			outtextxy_shaded(_pos_name_1P.x, _pos_name_1P.y, _str_name_gloomshroom);
			break;
		default:
			break;
		}

		switch (_player_type_2)
		{
		case PlayerType::PEASHOTER:
			_animation_peashoter.on_draw(camera, _pos_animation_2P.x, _pos_animation_2P.y);
			_pos_name_2P.x = _pos_img_gravestone_2P.x + (img_gravestone_left.getwidth() - textwidth(_str_name_peashoter)) / 2;
			outtextxy_shaded(_pos_name_2P.x, _pos_name_2P.y, _str_name_peashoter);
			break;
		case PlayerType::SUNFLOWER:
			_animation_sunflower.on_draw(camera, _pos_animation_2P.x, _pos_animation_2P.y);
			_pos_name_2P.x = _pos_img_gravestone_2P.x + (img_gravestone_left.getwidth() - textwidth(_str_name_sunflower)) / 2;
			outtextxy_shaded(_pos_name_2P.x, _pos_name_2P.y, _str_name_sunflower);
			break;
		case PlayerType::NUT:
			_animation_nut.on_draw(camera, _pos_animation_2P.x, _pos_animation_2P.y);
			_pos_name_2P.x = _pos_img_gravestone_2P.x + (img_gravestone_left.getwidth() - textwidth(_str_name_nut)) / 2;
			outtextxy_shaded(_pos_name_2P.x, _pos_name_2P.y, _str_name_nut);
			break;
		case PlayerType::GLOOMSHROOM:
			_animation_gloomshroom.on_draw(camera, _pos_animation_2P.x, _pos_animation_2P.y);
			_pos_name_2P.x = _pos_img_gravestone_2P.x + (img_gravestone_left.getwidth() - textwidth(_str_name_gloomshroom)) / 2;
			outtextxy_shaded(_pos_name_2P.x, _pos_name_2P.y, _str_name_gloomshroom);
			break;
		default:
			break;
		}

	}

	virtual void on_exit()
	{
		mciSendString(L"stop bgm_menu", nullptr, 0, nullptr);

		switch (_player_type_1)
		{
		case PlayerType::PEASHOTER:
			img_player_1_avatar = &img_avatar_peashooter;
			player_1 = std::shared_ptr<Player>(new PlayerPeashoter);
			break;
		case PlayerType::SUNFLOWER:
			img_player_1_avatar = &img_avatar_sunflower;
			player_1 = std::shared_ptr<Player>(new PlayerSunflower);
			break;
		case PlayerType::NUT:
			img_player_1_avatar = &img_avatar_nut;
			break;
		case PlayerType::GLOOMSHROOM:
			img_player_1_avatar = &img_avatar_gloomshroom;
			break;
		}
		player_1->set_playerID(PlayerID::P1);

		switch (_player_type_2)
		{
		case PlayerType::PEASHOTER:
			img_player_2_avatar = &img_avatar_peashooter;
			player_2 = std::shared_ptr<Player>(new PlayerPeashoter);
			break;
		case PlayerType::SUNFLOWER:
			img_player_2_avatar = &img_avatar_sunflower;
			player_2 = std::shared_ptr<Player>(new PlayerSunflower);
			break;
		case PlayerType::NUT:
			img_player_2_avatar = &img_avatar_nut;
			break;
		case PlayerType::GLOOMSHROOM:
			img_player_2_avatar = &img_avatar_gloomshroom;
			break;
		}
		player_2->set_playerID(PlayerID::P2);
	}
};