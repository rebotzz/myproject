#pragma once
#include <string>
#include "collision_manager.h"
#include "util.h"

// ���н������ܵĵ���
class InteractProp
{
private:
	CollisionBox* switch_box = nullptr;			// ������ײ����
	Vector2 position;							// λ��,����ê��ģʽ
	std::wstring desc;							// �����ı�
	CollisionBox* desc_box = nullptr;			// ������ײ����
	int desc_counter = 0;						// desc�����رռ�����
	bool is_showing_desc = false;				// �Ƿ���ʾ����
	bool is_showing_box = false;				// �Ƿ���ʾ��ײ����
	bool enabled = true;						// �Ƿ����õ���

	// ����,������ײ��߿����
	COLORREF color = RGB(200, 200, 200);
	int line_style = PS_SOLID;
	int line_thickness = 1;


public:
	InteractProp(const std::wstring& describe = _T(""))
		:desc(describe)
	{
		switch_box = CollisionManager::instance()->create_collision_box();
		switch_box->set_enabled(true);
		switch_box->set_layer_src(CollisionLayer::Enemy);	// ʹ��������������
		switch_box->set_layer_dst(CollisionLayer::None);

		desc_box = CollisionManager::instance()->create_collision_box();
		desc_box->set_enabled(true);
		desc_box->set_layer_src(CollisionLayer::Interact);
		desc_box->set_on_collision([&]()
			{
				is_showing_desc = true;
			});
	}
	~InteractProp()
	{
		CollisionManager::instance()->destroy_collision_box(switch_box);
		CollisionManager::instance()->destroy_collision_box(desc_box);
	}

	void on_render()
	{
		if (!enabled)
			return;

		if (is_showing_box)
		{
			setlinecolor(color);
			setlinestyle(line_style, line_thickness);
			rectangle((int)(position.x - switch_box->get_size().x / 2),
				(int)(position.y - switch_box->get_size().y / 2),
				(int)(position.x + switch_box->get_size().x / 2),
				(int)(position.y + switch_box->get_size().y / 2));
		}

		if (is_showing_desc)
		{
			settextcolor(RGB(255, 255, 255));
			outtextxy((int)position.x - textwidth(desc.c_str()) / 2,
				(int)(position.y - desc_box->get_size().y / 2) - textheight(desc.c_str()) - 10,
				desc.c_str());

			if (++desc_counter % 30 == 0)
				is_showing_desc = false;
		}
	}

	// ����ê��ģʽ
	void set_position(const Vector2& position)
	{
		this->position = position;
		switch_box->set_position(position);
		desc_box->set_position(position);
	}

	void set_size(const Vector2& size)
	{
		switch_box->set_size(size);
		desc_box->set_size(size);
	}

	void set_on_interact(const std::function<void()>& callback)
	{
		switch_box->set_on_collision(callback);
	}

	void set_describe(std::wstring str)
	{
		desc = str;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	void enable_showing_box(bool flag)
	{
		is_showing_box = flag;
	}

	void set_showing_style(COLORREF col = RGB(200, 200, 200), int line_style = PS_SOLID, int line_thickness = 1)
	{
		color = col;
		this->line_style = line_style;
		this->line_thickness = line_thickness;
	}

	void set_enabled(bool flag)
	{
		enabled = flag;
		switch_box->set_enabled(flag);
		desc_box->set_enabled(flag);
	}
};

