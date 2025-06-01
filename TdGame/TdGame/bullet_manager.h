#pragma once

#include "singleton.h"
#include "bullet.h"
#include "bulltet_arrow.h"
#include "bullet_axe.h"
#include "bullet_shell.h"

#include <vector>

enum class BulletType
{
	Arrow,
	Axe,
	Shell
};

class BulletMgr : public Singleton<BulletMgr>
{
	friend class  Singleton<BulletMgr>;
	typedef std::vector<Bullet*> BulletList;

public:
	void on_update(float delta)
	{
		for (Bullet* bullet : bullet_list)
			bullet->on_update(delta);

		// 超出屏幕边界设置为无效
		for (Bullet* bullet : bullet_list)
		{
			if (bullet->can_remove()) continue;
			const Vector2& position = bullet->get_position();
			static int window_w = ConfigMgr::instance()->window_width;
			static int window_h = ConfigMgr::instance()->window_height;
			if (position.x < 0 || position.x > window_w || position.y < 0 || position.y > window_h)
				bullet->make_invalid();
		}

		// 移除无效
		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(),
			[](Bullet* bullet)
			{
				bool deletable = bullet->can_remove();
				if (deletable) delete bullet;
				return deletable;
			}), bullet_list.end());
	}

	void on_renderer(SDL_Renderer* renderer)
	{
		for (Bullet* bullet : bullet_list)
			bullet->on_render(renderer);
	}

	void create_bullet(BulletType type, const Vector2& direction, const Vector2& position, int damage)
	{
		Bullet* bullet = nullptr;
		switch (type)
		{
		case BulletType::Arrow:
			bullet = new BulletArrow(direction, position);
			break;
		case BulletType::Axe:
			bullet = new BulletAxe(direction, position);
			break;
		case BulletType::Shell:
			bullet = new BulletShell(direction, position);
			break;
		}
		bullet->set_damage(damage);
		bullet_list.push_back(bullet);
	}

protected:
	BulletMgr() = default;
	~BulletMgr() = default;

private:
	BulletList bullet_list;
};
