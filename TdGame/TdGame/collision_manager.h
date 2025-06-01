#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

#include "singleton.h"
#include "collision_box.h"

#include <vector>
#include "SDL2_gfxPrimitives.h"

// ��ײ������	����+�۲���
class CollisionMgr : public Singleton<CollisionMgr>
{
	friend  Singleton<CollisionMgr>;
	typedef std::vector<CollisionBox*> CollisionBoxList;

public:
	CollisionBox* create_collision_box()
	{
		CollisionBox* box = new CollisionBox();
		collision_box_list.push_back(box);
		return box;
	}

	void destroy_collision_box(CollisionBox* collision_box)
	{
		if (collision_box == nullptr) return;
		collision_box_list.erase(std::remove(collision_box_list.begin(), collision_box_list.end(), collision_box),
			collision_box_list.end());
		delete collision_box;
	}

	void on_update(float delta)
	{
		// todo:������԰�һ����������ײ������ÿ֡�����
	}

	void on_debug_render(SDL_Renderer* renderer)
	{
		for (CollisionBox* box : collision_box_list)
		{
			if (box->get_shape_mode() == CollisionBox::ShapeMode::Rect)
			{
				if (box->get_valid())
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				else
					SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

				SDL_Rect rect = {
					(int)(box->get_postion().x - box->get_size().x / 2),
					(int)(box->get_postion().y - box->get_size().y / 2),
					(int)(box->get_size().x),
					(int)(box->get_size().y) };
				SDL_RenderDrawRect(renderer, &rect);
			}
			else
			{
				if (box->get_valid())
					aacircleColor(renderer, box->get_postion().x, box->get_postion().y, box->get_radius(), 0xff0000ff);
				else
					aacircleColor(renderer, box->get_postion().x, box->get_postion().y, box->get_radius(), 0xffffff00);
			}
		}
	}

	void process_collision()
	{
		// ��ײ������
		for (CollisionBox* src_box : collision_box_list)
		{
			if (!src_box->get_valid() || src_box->dst_layer == CollisionLayer::None)
				continue;

			// ��ײ���շ�
			for (CollisionBox* dst_box : collision_box_list)
			{
				if (src_box == dst_box || !dst_box->get_valid() || dst_box->src_layer == CollisionLayer::None
					|| src_box->dst_layer != dst_box->src_layer)	// Ŀǰ����һ����ײ���ж�
					continue;
				if (!src_box->get_valid()) break;					// �е���ײ����ֻ��ײһ�ξ�ʧЧ

				bool is_collide = false;
				auto& src_pos = src_box->get_postion();
				auto& dst_pos = dst_box->get_postion();
				// Բ����ཻ�ж�
				if (src_box->get_shape_mode() == CollisionBox::ShapeMode::Circle)
				{
					if ((src_pos - dst_pos).lenth() < src_box->get_radius())
						is_collide = true;
				}
				else
				{
					// �����ཻ�ж�
					auto& src_size = src_box->get_size();
					auto& dst_size = dst_box->get_size();
					int top_y = std::min(src_pos.y - src_size.y / 2, dst_pos.y - dst_size.y / 2);
					int bottom_y = std::max(src_pos.y + src_size.y / 2, dst_pos.y + dst_size.y / 2);
					int left_x = std::min(src_pos.x - src_size.x / 2, dst_pos.x - dst_size.x / 2);
					int right_x = std::max(src_pos.x + src_size.x / 2, dst_pos.x + dst_size.x / 2);
					if (std::abs(top_y - bottom_y) < src_size.y + dst_size.y
						&& std::abs(left_x - right_x) < src_size.x + dst_size.x)			// debug: ֮ǰд����
					{
						is_collide = true;
					}
				}

				if (is_collide)
				{
					// ��¼��ֻ��ײ����ײ���ӣ�������ײ�����߼�
					src_box->set_on_collide_box(dst_box);
					dst_box->set_on_collide_box(src_box);
					src_box->on_collide();
					dst_box->on_collide();
				}
			}
		}
	}

protected:
	CollisionMgr() = default;
	~CollisionMgr() = default;

private:
	CollisionBoxList collision_box_list;
};




#endif // !_COLLISION_MANAGER_H_



