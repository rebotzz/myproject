#include "bartend_bottle.h"

#include "cursor_manager.h"
#include "resources_manager.h"
#include "kits.h"
#include "game_system.h"
#include "bartend_system.h"
#include "dialogue_manager.h"
#include <unordered_set>
#include <algorithm>
#include <cmath>

BartendBottle::BartendBottle() :Region({ 980, 450, 100, 120 })
{
	timer_shake.set_wait_time(SHAKE_CD);
	timer_shake.set_one_shot(true);
	timer_shake.set_on_timeout([&]
		{
			if (Status::Shaking == status)
			{
				status = Status::SevereShaking;
				timer_anim.set_wait_time(SHAKE_FRAME_DELTA * 0.5);
			}
		});
	timer_anim.set_one_shot(false);
	timer_anim.set_wait_time(SHAKE_FRAME_DELTA);
	timer_anim.set_on_timeout([&]() {
		frame_idx = (frame_idx + 1) % frame_list.size();
		});

	tex_open = ResMgr::instance()->find_texture("bottle_open");
	tex_close = ResMgr::instance()->find_texture("bottle_close");

	// 初始化Trie树
	std::vector<Meal> sugar_rush_list = { Meal::Adelhyde, Meal::Adelhyde, Meal::PwdDelta };	
	std::vector<Meal> flff_dream_list = { Meal::Adelhyde, Meal::Adelhyde, Meal::Adelhyde,
		Meal::PwdDelta, Meal::PwdDelta, Meal::PwdDelta, Meal::Ageing };								
	std::vector<Meal> moon_blast_list = { Meal::Adelhyde, Meal::Adelhyde,  Meal::Adelhyde, Meal::Adelhyde,
	 Meal::Adelhyde, Meal::Adelhyde, Meal::PwdDelta, Meal::Flanergide, Meal::Karmotrine, Meal::Karmotrine, Meal::Ice, Meal::Shaking };
	std::vector<Meal> coblt_vlvt_list = { Meal::Adelhyde, Meal::Adelhyde, Meal::Flanergide, Meal::Flanergide,
	Meal::Flanergide, Meal::Karmotrine, Meal::Karmotrine, Meal::Karmotrine, Meal::Karmotrine, Meal::Karmotrine, Meal::Ice };

	tree.add_branch(sugar_rush_list, Meal::SugarRush, true);// 任选酒精
	tree.add_branch(flff_dream_list, Meal::FlffDream, true);// 任选酒精
	tree.add_branch(moon_blast_list, Meal::MoonBlast);
	tree.add_branch(coblt_vlvt_list, Meal::CobltVlvt);
}

void BartendBottle::on_update(float delta)
{
	if (Status::Shaking == status)
		timer_shake.on_update(delta);
	if (Status::Shaking == status || Status::SevereShaking == status)
		timer_anim.on_update(delta);
}
void BartendBottle::on_render(SDL_Renderer* renderer)
{
	// 绘制调酒瓶
	if (Status::Init == status)
	{
		if (SDL_PointInRect(&CursorMgr::instance()->get_position(), &rect) && CursorMgr::instance()->get_picked() != Meal::None)
		{
			render_img(renderer, tex_open, { rect.x + rect.w / 2, rect.y + rect.h / 2 });
		}
		else
		{
			render_img(renderer, tex_close, { rect.x + rect.w / 2, rect.y + rect.h / 2 });
		}
	}
	// 绘制摇晃的调酒瓶动画
	else if (Status::SevereShaking == status || Status::Shaking == status)
	{
		double angle = frame_list[frame_idx];
		render_img(renderer, tex_close, { rect.x + rect.w / 2, rect.y + rect.h / 2 }, angle);
	}
	// 绘制调好的酒
	else
	{
		SDL_Texture* tex_drink = nullptr;
		switch (drink)
		{
		default:
		case Meal::Unkown: tex_drink = ResMgr::instance()->find_texture("unkown"); break;
		case Meal::CobltVlvt: tex_drink = ResMgr::instance()->find_texture("coblt_vlvt"); break;
		case Meal::FlffDream: tex_drink = ResMgr::instance()->find_texture("flff_dream"); break;
		case Meal::MoonBlast: tex_drink = ResMgr::instance()->find_texture("moonblast"); break;
		case Meal::SugarRush: tex_drink = ResMgr::instance()->find_texture("sugar_rush"); break;
		}
		render_img(renderer, tex_drink, { rect.x + rect.w / 2, rect.y + rect.h / 2 });
	}

	// 绘制材料数量
	if (status != Status::Done)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect base_rect = { 960 + 10, 580, 120 / 5 - 2, 20 / 2 - 2 };
		for (int i = 0; i < materials.size(); ++i)
		{
			SDL_Rect count_rect = base_rect;
			count_rect.x += (i % 5) * (base_rect.w + 3) + 2;
			count_rect.y += ((i / 5) % 2) * (base_rect.h + 5) + 4;

			if (i >= 10) SDL_RenderFillRect(renderer, &count_rect);
			else SDL_RenderDrawRect(renderer, &count_rect);
		}
	}
	// 绘制酒的名字
	else
	{
		std::string str;
		switch (drink)
		{
		default:
		case Meal::Unkown: str = "unkown??"; break;
		case Meal::CobltVlvt: str = "Coblt Vlvt"; break;
		case Meal::FlffDream: str = "Flff Dream"; break;
		case Meal::MoonBlast: str = "Moon Blast"; break;
		case Meal::SugarRush: str = "Sugar Rush"; break;
		}
		SDL_Point text_center_point = { 960 + 136 / 2, 580 + 26 / 2 };
		render_text(renderer, str, text_center_point, { 255,255,255,255 }, "simhei", 0.7);
	}
}

void BartendBottle::on_cursor_up()
{
	if (materials.size() >= 20) return;
	static std::unordered_set<Meal> allow_list = { Meal::Adelhyde, Meal::Adelhyde, Meal::BronsonExt, Meal::PwdDelta,
		Meal::Flanergide, Meal::Karmotrine };
	if (Meal::Ice == CursorMgr::instance()->get_picked() && !has_ice)
	{
		has_ice = true;
		materials.push_back(Meal::Ice);
		CursorMgr::instance()->set_picked(Meal::None);
	}
	else if (Meal::Ageing == CursorMgr::instance()->get_picked() && !is_ageing)
	{
		is_ageing = true;
		materials.push_back(Meal::Ageing);
		CursorMgr::instance()->set_picked(Meal::None);
	}
	else if (allow_list.count(CursorMgr::instance()->get_picked()))
	{
		materials.push_back(CursorMgr::instance()->get_picked());
		CursorMgr::instance()->set_picked(Meal::None);
	}
}

// 清空调酒瓶
void BartendBottle::reset()
{
	is_ageing = false;
	has_ice = false;
	status = Status::Init;
	timer_shake.restart();
	timer_anim.set_wait_time(SHAKE_FRAME_DELTA);
	materials.clear();
}
// 调制饮料
void BartendBottle::modulate()	
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
		// 判断酒是否对
		// 符合：设置完成目标  不符合：提示不对，不完成目标
		if (CursorMgr::instance()->get_drink_goal() == drink)
		{
			GameSystem::instance()->finish_goal();
			DialogMgr::instance()->enable_tips(false);
			BartendSystem::instance()->enable_meun(false);
			CursorMgr::instance()->add_coins(10);
		}
		else
		{
			std::string tips = u8"Tips: 需要调制的饮品似乎不是这个,再瞅一眼配方：";
			tips += CursorMgr::instance()->get_drink_goal_name();
			DialogMgr::instance()->set_tips(tips);
			DialogMgr::instance()->enable_tips(true);
		}
		BartendSystem::instance()->reset();
		break;
	}
}

void BartendBottle::shake()
{
	status = Status::Shaking;
	timer_shake.restart();
	timer_anim.set_wait_time(SHAKE_FRAME_DELTA);
}
void BartendBottle::stop_shaking()
{
	if (Status::SevereShaking == status)
	{
		materials.push_back(Meal::Shaking);
	}
	status = Status::Done;
	drink = tree.check(materials);

#ifdef DEBUG
	SDL_Log("Adelhyde: %d\n", (int)Meal::Adelhyde);
	SDL_Log("materials size: %d\n", (int)materials.size());
	for (auto x : materials)
	{
		SDL_Log("%d ", (int)x);
	}
	SDL_Log("\n");
#endif // DEBUG
}
Meal BartendBottle::get_drink()
{
	return drink;
}


// 检测配方是否正确，返回饮料种类
Meal BartendBottle::TrieTree::check(std::vector<Meal>& material_list)
{
	// 先排序（升序）材料，然后trie树
	std::sort(material_list.begin(), material_list.end());
	auto cur = root;
	bool has_ice = false, has_ageing = false;
	for (int i = 0; i < material_list.size(); ++i)
	{
		if (material_list[i] == Meal::Ice)
		{
			has_ice = true;
		}
		else if (material_list[i] == Meal::Ageing)
		{
			has_ageing = true;
		}
		else
		{
			if (cur->nexts.count(material_list[i])) cur = cur->nexts[material_list[i]];
			else return Meal::Unkown;
		}
	}
	if (has_ice != cur->need_ice || has_ageing != cur->need_ageing) return Meal::Unkown;
	//if ((has_ice && !cur->need_ice) || (has_ageing && !cur->need_ageing)
	//	|| (!has_ice && cur->need_ice) || (!has_ageing && cur->need_ageing))
	//	return Meal::Unkown;
	return cur->drink;
}
// 添加新的配方
void BartendBottle::TrieTree::add_branch(std::vector<Meal>& material_list, Meal target, bool any_karmotrine)
{
	std::sort(material_list.begin(), material_list.end());	// 排序，保证唯一性
	bool need_ice = false, need_ageing = false;
	Node* cur = root;
	for (int i = 0; i < 20; ++i)
	{
		if (i < material_list.size())
		{
			if (material_list[i] == Meal::Ice)
			{
				cur->need_ice = true;
				need_ice = true;
			}
			else if (material_list[i] == Meal::Ageing)
			{
				cur->need_ageing = true;
				need_ageing = true;
			}
			else
			{
				if (!cur->nexts.count(material_list[i]))
				{
					cur->nexts[material_list[i]] = new Node;
				}
				cur = cur->nexts[material_list[i]];
			}
		}
		else if (any_karmotrine)
		{
			cur->drink = target;
			cur->need_ageing = need_ageing;
			cur->need_ice = need_ice;
			if (!cur->nexts.count(Meal::Karmotrine))
			{
				cur->nexts[Meal::Karmotrine] = new Node(target);
			}
			cur = cur->nexts[Meal::Karmotrine];
		}
		else break;

	}
	cur->drink = target;
	cur->need_ageing = need_ageing;
	cur->need_ice = need_ice;
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





