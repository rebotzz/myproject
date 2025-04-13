#include "bartending_tools.h"
#include "cursor_manager.h"
#include "resources_manager.h"
#include <unordered_set>
#include <algorithm>

RawMaterial::RawMaterial(Meal _meal, SDL_Rect _rect, SDL_Color _color)
	:Region(_rect), meal(_meal), color(_color)
{
}

void RawMaterial::on_render(SDL_Renderer* renderer)
{
	// 绘制表示原料加入份量的矩形
	SDL_Rect base_rect = { rect.x + 50, rect.y, 90 / 5, 70 / 2 };
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a * 0.5);
	for (int i = 0; i < 10; ++i)
	{
		SDL_Rect count_rect = base_rect;
		if (i < 5) count_rect.x += (i % 5) * base_rect.w + 2;
		if (i > 5) count_rect.y += base_rect.h + 2;

		SDL_RenderDrawRect(renderer, &count_rect);
	}
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int i = 0; i < count; ++i)
	{
		SDL_Rect count_rect = base_rect;
		count_rect.x += (i % 5) * base_rect.w + 2;
		count_rect.y += ((i / 5) % 2) * base_rect.h + 2;

		if (i >= 10) SDL_RenderFillRect(renderer, &count_rect);
		else SDL_RenderDrawRect(renderer, &count_rect);
	}
}
void RawMaterial::on_cursor_down()
{
	if (CursorMgr::instance()->get_picked() == Meal::None)
		CursorMgr::instance()->set_picked(meal);
}
void RawMaterial::on_cursor_up()
{
	if (CursorMgr::instance()->get_picked() == meal)
		CursorMgr::instance()->set_picked(Meal::None);
}

BartendBottle::BartendBottle()
{
	timer_shake.set_one_shot(true);
	timer_shake.set_on_timeout([&]
		{
			if (Status::Shaking == status)
			{
				status = Status::SevereShaking;
				anim.set_interval(0.2);
			}
		});

	tex_open = ResMgr::instance()->find_texture("bottle_open");
	tex_close = ResMgr::instance()->find_texture("bottle_close");

	anim.set_interval(0.5);
	anim.set_loop(true);
	anim.set_position({ rect.x + rect.w / 2, rect.y + rect.h });
	anim.add_frame(ResMgr::instance()->find_texture("bottle_close"));
	anim.add_frame(ResMgr::instance()->find_texture("bottle_left"));
	anim.add_frame(ResMgr::instance()->find_texture("bottle_right"));

	// 初始化Trie树
	static std::vector<Meal> sugar_rush_list = { Meal::Adelhyde, Meal::Adelhyde, Meal::PwdDelta };	// 任选酒精
	static std::vector<Meal> flff_dream_list = { Meal::Adelhyde, Meal::Adelhyde, Meal::Adelhyde,
		Meal::PwdDelta, Meal::PwdDelta, Meal::PwdDelta, Meal::Ageing };								// 任选酒精
	static std::vector<Meal> moon_blast_list = { Meal::Adelhyde, Meal::Adelhyde,  Meal::Adelhyde, Meal::Adelhyde,
	 Meal::Adelhyde, Meal::Adelhyde, Meal::PwdDelta, Meal::Flanergide, Meal::Karmotrine, Meal::Karmotrine, Meal::Ice };
	static std::vector<Meal> coblt_vlvt_list = { Meal::Adelhyde, Meal::Adelhyde, Meal::Flanergide, Meal::Flanergide, 
	Meal::Flanergide, Meal::Karmotrine, Meal::Karmotrine, Meal::Karmotrine, Meal::Karmotrine, Meal::Karmotrine, Meal::Ice };


	tree.add_branch(sugar_rush_list, Meal::SugarRush, true);
	tree.add_branch(flff_dream_list, Meal::FlffDream, true);
	tree.add_branch(moon_blast_list, Meal::MoonBlast);
	tree.add_branch(coblt_vlvt_list, Meal::CobltVlvt);

}

void BartendBottle::on_update(float delta)
{
	if (Status::Shaking == status)
		timer_shake.on_update(delta);
}
void BartendBottle::on_render(SDL_Renderer* renderer)
{
	if (Status::SevereShaking == status || Status::Shaking == status)
	{
		anim.on_render(renderer);
	}
	else if (SDL_PointInRect(&CursorMgr::instance()->get_position(), &rect))
	{
		SDL_RenderCopy(renderer, tex_open, nullptr, &rect);
	}
	else
	{
		SDL_RenderCopy(renderer, tex_close, nullptr, &rect);
	}
}
void BartendBottle::on_cursor_up()
{
	static std::unordered_set<Meal> allow_list = { Meal::Adelhyde, Meal::Adelhyde, Meal::BronsonExt, Meal::PwdDelta,
		Meal::Flanergide, Meal::Karmotrine, Meal::Ice, Meal::Ageing, Meal::Shaking };
	if (allow_list.count(CursorMgr::instance()->get_picked()))
	{
		materials.push_back(CursorMgr::instance()->get_picked());
		CursorMgr::instance()->set_picked(Meal::None);
	}
}

// 清空调酒瓶
void BartendBottle::reset()
{
	anim.reset();
	anim.set_interval(0.5);
	materials.clear();
}
void BartendBottle::shake()
{
	status = Status::Shaking;
	timer_shake.restart();
}
void BartendBottle::stop_shaking()
{
	if (Status::SevereShaking == status)
		materials.push_back(Meal::Shaking);

	status = Status::Done;
}
Meal BartendBottle::get_drink()
{
	// 先排序（升序）材料，然后trie树
	std::sort(materials.begin(), materials.end());
	return tree.check(materials);
}


// 检测配方是否正确，返回饮料种类
Meal BartendBottle::TrieTree::check(const std::vector<Meal>& arr)
{
	auto cur = root;
	int i = 0;
	for (; i < arr.size(); ++i)
	{
		if (cur->nexts.count(arr[i])) cur = cur->nexts[arr[i]];
		else break;
	}
	if (i == arr.size()) return cur->drink;
}
// 添加新的配方
void BartendBottle::TrieTree::add_branch(const std::vector<Meal>& arr, Meal meal, bool any_karmotrine = false)
{
	Node* cur = root;
	for (int i = 0; i < 20; ++i)
	{
		if (i < arr.size())
		{
			if (!cur->nexts.count(arr[i]))
			{
				cur->nexts[arr[i]] = new Node;
			}
			cur = cur->nexts[arr[i]];
		}
		else if (any_karmotrine)
		{
			if (!cur->nexts.count(Meal::Karmotrine))
			{
				cur->nexts[Meal::Karmotrine] = new Node(meal);
			}
			cur = cur->nexts[Meal::Karmotrine];
		}
		else break;

	}
	cur->drink = meal;
}


BartendBottle::TrieTree::TrieTree()
{
	root = new Node;
}
BartendBottle::TrieTree::~TrieTree()
{
	// 释放资源 dfs
	destroy(root);
}

void BartendBottle::TrieTree::destroy(Node* node)
{
	if (node == nullptr) return;
	for (auto iter : node->nexts)
	{
		Node* ptr = iter.second;
		if (ptr)
		{
			destroy(ptr);
		}
	}
	delete node;
}

