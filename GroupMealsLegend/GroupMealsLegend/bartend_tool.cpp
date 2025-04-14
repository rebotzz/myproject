#include "bartend_tool.h"
#include "cursor_manager.h"
#include "resources_manager.h"
#include "kits.h"
#include <unordered_set>
#include <algorithm>



BartendBottle::BartendBottle():Region({990, 420, 100, 120})
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
	if (Status::Init == status)
	{
		SDL_Rect rect_img{ 0 };
		if (SDL_PointInRect(&CursorMgr::instance()->get_position(), &rect))
		{
			SDL_QueryTexture(tex_open, nullptr, nullptr, &rect_img.w, &rect_img.h);
			rect_img.x = rect.x + rect.h - rect_img.h;
			rect_img.y = rect.y + rect.w / 2 - rect_img.w / 2;
			SDL_RenderCopy(renderer, tex_open, nullptr, &rect_img);
		}
		else
		{
			SDL_QueryTexture(tex_close, nullptr, nullptr, &rect_img.w, &rect_img.h);
			rect_img.x = rect.x + rect.h - rect_img.h;
			rect_img.y = rect.y + rect.w / 2 - rect_img.w / 2;
			SDL_RenderCopy(renderer, tex_close, nullptr, &rect_img);
		}
	}
	else if (Status::SevereShaking == status || Status::Shaking == status)
	{
		anim.on_render(renderer);
	}
	else
	{
		SDL_Texture* tex = nullptr;
		switch (drink)
		{
		case Meal::Unkown: tex = ResMgr::instance()->find_texture("unkown"); break;
		case Meal::CobltVlvt: tex = ResMgr::instance()->find_texture("coblt_vlvt"); break;
		case Meal::FlffDream: tex = ResMgr::instance()->find_texture("flff_dream"); break;
		case Meal::MoonBlast: tex = ResMgr::instance()->find_texture("moonblast"); break;
		case Meal::SugarRush: tex = ResMgr::instance()->find_texture("sugar_rush"); break;
		}
		SDL_Rect rect_img{ 0 };
		SDL_QueryTexture(tex_close, nullptr, nullptr, &rect_img.w, &rect_img.h);
		rect_img.x = rect.x + rect.h - rect_img.h;
		rect_img.y = rect.y + rect.w / 2 - rect_img.w / 2;
		SDL_RenderCopy(renderer, tex, nullptr, &rect_img);
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
void BartendBottle::modulate()	// 调制饮料
{
	switch (status)
	{
	case Status::Init: 
		shake(); 
		break;
	case Status::Shaking:
	case Status::SevereShaking:
		stop_shaking(); 
		break;
	case Status::Done:
		if (CursorMgr::instance()->get_picked() == Meal::None)
		{
			CursorMgr::instance()->set_picked(get_drink());
		}
		break;
	}
}


void BartendBottle::shake()
{
	status = Status::Shaking;
	timer_shake.restart();
}
void BartendBottle::stop_shaking()
{
	if (Status::SevereShaking == status || Status::Shaking == status)
	{
		if(Status::SevereShaking == status)
			materials.push_back(Meal::Shaking);
		status = Status::Done;
		// 先排序（升序）材料，然后trie树
		std::sort(materials.begin(), materials.end());
		drink = tree.check(materials);
	}

}
Meal BartendBottle::get_drink()
{
	return drink;
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
void BartendBottle::TrieTree::add_branch(const std::vector<Meal>& arr, Meal meal, bool any_karmotrine)
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





