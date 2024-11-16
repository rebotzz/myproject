#pragma once
#include "scene_manager.h"
#include "collision_manager.h"
#include "timer.h"

// ������
class Door
{
private:
	CollisionBox* door_box = nullptr;			// ��������ײ����
	CollisionBox* describe_box = nullptr;		// ������������ײ����
	std::string dst_scene;						// Ŀ�곡��
	std::wstring describe;						// ����Ŀ������
	Vector2 position;							// λ��,����ê��ģʽ
	bool is_showing_desc = false;				// �Ƿ���ʾ����
	bool is_enter = false;						// �Ƿ������,�����ظ���ײ����
	COLORREF color = RGB(0, 255, 255);			// �ſ���ɫ
	int showing_counter = 0;					// �ر�����������
	Timer timer_enter;

public:
	Door(const std::string& destination)
		:dst_scene(destination)
	{
		door_box = CollisionManager::instance()->create_collision_box();
		door_box->set_enabled(true);
		door_box->set_layer_src(CollisionLayer::Sword);
		door_box->set_on_collision([&]()
			{
				is_enter = true;
			});

		describe_box = CollisionManager::instance()->create_collision_box();
		describe_box->set_enabled(true);
		describe_box->set_layer_src(CollisionLayer::Interact);
		describe_box->set_on_collision([&]()
			{
				is_showing_desc = true;
			});

		timer_enter.set_one_shot(true);
		timer_enter.set_wait_time(0.5f);
		timer_enter.set_on_timeout([&]()
			{
				SceneManager::instance()->set_transition_next_scene(dst_scene);
				SceneManager::instance()->switch_scene("transition_scene");
			});
	}
	~Door()
	{
		CollisionManager::instance()->destroy_collision_box(door_box);
		CollisionManager::instance()->destroy_collision_box(describe_box);
	}

	void set_color(COLORREF val)
	{
		color = val;
	}

	void set_describe(const std::wstring& desc)
	{
		describe = desc;
	}

	void set_size(const Vector2& size)
	{
		door_box->set_size(size);
		describe_box->set_size(size);
	}

	// ����ê��ģʽ
	void set_position(const Vector2& position)
	{
		this->position = position;
		door_box->set_position(position);
		describe_box->set_position(position);
	}

	void on_update(float delta)
	{
		if (is_enter)
			timer_enter.on_update(delta);
	}

	void on_render()
	{
		setlinestyle(PS_SOLID, 2);
		setlinecolor(color);
		rectangle((int)(position.x - door_box->get_size().x / 2),
			(int)(position.y - door_box->get_size().y / 2),
			(int)(position.x + door_box->get_size().x / 2),
			(int)(position.y + door_box->get_size().y / 2));

		if (is_showing_desc)
		{
			settextcolor(RGB(255, 255, 255));
			outtextxy((int)position.x - textwidth(describe.c_str()) / 2,
				(int)position.y - (int)door_box->get_size().y / 2 - textheight(describe.c_str()) - 10,
				describe.c_str());

			if (++showing_counter % 30 == 0)
				is_showing_desc = false;
		}
	}
};