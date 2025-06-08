#pragma once
#include "resources_manager.h"
#include "interact_prop.h"

// ľͷ��
class Woodenman
{
private:
	IMAGE* img = nullptr;						// ľͷ����ͼ
	InteractProp prop;							// ľͷ�˽�������
	int showing_hit_counter = 0;				// �ر�hit����������
	bool is_showing_hit = false;				// �Ƿ���ʾhit����

public:
	Woodenman()
		:prop(_T("����ľͷ��"))
	{
		img = ResourcesManager::instance()->find_image("woodenman");
		prop.set_on_interact([&](CollisionBox*)
			{
				is_showing_hit = true;
			});
		prop.set_size({ (float)img->getwidth() - 10, (float)img->getheight() - 20 });
	}
	~Woodenman()
	{}

	// ����ê��ģʽ
	void set_position(const Vector2& position)
	{
		prop.set_position(position);
	}

	Vector2 get_img_size() const
	{
		return { (float)img->getwidth(), (float)img->getheight() };;
	}

	void on_render()
	{
		putimage_alpha((int)prop.get_position().x - img->getwidth() / 2,
			(int)prop.get_position().y - img->getheight() / 2, img);

		prop.on_render();

		if (is_showing_hit)
		{
			settextcolor(RGB(255, 50, 50));
			outtextxy((int)prop.get_position().x - textwidth(_T("hp -1")) / 2,
				(int)prop.get_position().y - img->getheight() / 2 - textheight(_T("hp -1")) - 10,
				_T("hp -1"));

			if (++showing_hit_counter % 30 == 0)
				is_showing_hit = false;
		}
	}

	void set_enabled(bool flag)
	{
		prop.set_enabled(flag);
	}
};