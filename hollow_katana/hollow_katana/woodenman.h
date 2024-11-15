#pragma once
#include "collision_manager.h"
#include "resources_manager.h"
#include "util.h"

// ������
class Woodenman
{
private:
	IMAGE* img = nullptr;						// ľͷ����ͼ
	CollisionBox* hurt_box = nullptr;			// ľͷ����ײ����
	Vector2 position;							// λ��,����ê��ģʽ

	CollisionBox* describe_box = nullptr;		// ������ײ����
	int showing_desc_counter = 0;				// �ر�desc����������
	bool is_showing_desc = false;				// �Ƿ���ʾľͷ������
	int showing_hit_counter = 0;				// �ر�hit����������
	bool is_showing_hit = false;				// �Ƿ���ʾhit����

public:
	Woodenman()
	{
		img = ResourcesManager::instance()->find_image("woodenman");
		hurt_box = CollisionManager::instance()->create_collision_box();
		hurt_box->set_enabled(true);
		hurt_box->set_layer_src(CollisionLayer::Enemy);
		hurt_box->set_layer_dst(CollisionLayer::None);
		hurt_box->set_on_collision([&]()
			{
				is_showing_hit = true;
			});
		hurt_box->set_size({ (float)img->getwidth() - 10, (float)img->getheight() - 20 });

		describe_box = CollisionManager::instance()->create_collision_box();
		describe_box->set_enabled(true);
		describe_box->set_layer_src(CollisionLayer::Scenery);
		describe_box->set_size({ (float)img->getwidth() - 10, (float)img->getheight() - 20 });
		describe_box->set_on_collision([&]()
			{
				is_showing_desc = true;
			});
	}
	~Woodenman()
	{
		CollisionManager::instance()->destroy_collision_box(hurt_box);
		CollisionManager::instance()->destroy_collision_box(describe_box);
	}

	// ����ê��ģʽ
	void set_position(const Vector2& position)
	{
		this->position = position;
		hurt_box->set_position(position);
		describe_box->set_position(position);
	}

	Vector2 get_img_size() const
	{
		return { (float)img->getwidth(), (float)img->getheight() };;
	}

	void on_render()
	{
		putimage_alpha((int)position.x - img->getwidth() / 2,
			(int)position.y - img->getheight() / 2, img);

		if (is_showing_desc)
		{
			settextcolor(RGB(255, 255, 255));
			outtextxy((int)position.x - textwidth(_T("����ľͷ��")) / 2,
				(int)position.y - img->getheight() / 2 - textheight(_T("����ľͷ��")) - 10,
				_T("����ľͷ��"));

			if (++showing_desc_counter % 30 == 0)
				is_showing_desc = false;
		}

		if (is_showing_hit)
		{
			settextcolor(RGB(255, 50, 50));
			outtextxy((int)position.x - textwidth(_T("hp -1")) / 2,
				(int)position.y - img->getheight() / 2 - textheight(_T("hp -1")) - 10,
				_T("hp -1"));

			if (++showing_hit_counter % 30 == 0)
				is_showing_hit = false;
		}

	}
};