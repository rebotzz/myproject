#include "bartend_meun.h"
#include "region_manager.h"
#include "resources_manager.h"
#include "cursor_manager.h"
#include "kits.h"
#include <unordered_map>

BartendMeun::BartendMeun()
{
	rect = { 0, 155, 700, 469 };	// 菜单所在矩形
	SDL_Rect rect_prev = { rect.x + 30, rect.y + rect.h - 80 , 0 ,0 };
	SDL_QueryTexture(ResMgr::instance()->find_texture("button_prev"), nullptr, nullptr, &rect_prev.w, &rect_prev.h);
	button_prev.set_rect(rect_prev);
	button_prev.set_texture(ResMgr::instance()->find_texture("button_prev"));
	button_prev.set_on_click([this]()
		{
			idx--;
			if (idx < 0) idx = 0;
		});

	SDL_Rect rect_next = { rect.x + rect.w - 100, rect_prev.y , 0 ,0 };
	SDL_QueryTexture(ResMgr::instance()->find_texture("button_next"), nullptr, nullptr, &rect_next.w, &rect_next.h);
	button_next.set_rect(rect_next);
	button_next.set_texture(ResMgr::instance()->find_texture("button_next"));
	button_next.set_on_click([this]()
		{
			idx++;
			if (idx > formula_list.size()) idx = formula_list.size();
		});


	RegionMgr::instance()->add("btc_meun", this, 0);
	RegionMgr::instance()->add("button_prev", &button_prev, 1);
	RegionMgr::instance()->add("button_next", &button_next, 1);
	set_valid(false);

	rect_btc_icon = { rect.x + 100, rect.y + 150, 0, 0 };
	SDL_QueryTexture(ResMgr::instance()->find_texture("btc_icon"), nullptr, nullptr, &rect_btc_icon.w, &rect_btc_icon.h);

	formula_list[0] = { u8"Sugar Rush - $150",
	u8"Sugar Rush是由2 Adelhyde、1 Powdered Delta和",
	u8"任选Karmotrine调制而成。",
	u8"\"甘甜，清淡，水果风味，不能更加女性化的饮品。\"",
	u8"甜味，女性化，惬意。"
	};
	formula_list[1] = { u8"Fluffy Dream - $170",
	u8"Fluffy Dream是由3 Adelhyde、3 Powdered Delta和",
	u8"任选Karmotrine陈化，调制而成。",
	u8"\"一两口就足以取悦你的舌头，再多喝就可能会导致",
	u8"睡过头。\"",
	u8"酸味，女性化，温和。"
	};
	formula_list[2] = { u8"Moonblast - $180",
	u8"Moonblast是由6 Adelhyde、1 Powdered Delta、",
	u8"1 Flanergide 和2 Karmotrine加冰，调和而成。",
	u8"(点击\"调制\"按钮后请等待至摇酒壶开始剧烈晃动。)",
	u8"\"与你每个月都有一周时间能看到的那座月球强子大",	
	u8"炮没有任何关系。\"",
	u8"甜味，女性化，惬意。"
	};
	formula_list[3] = { u8"Cobalt Velvet - $280",
	u8"Cobalt Velvet是由2 Adelhyde、3 Flanergide和",
	u8"5 Karmotrine加冰，调制而成。",
	u8"\"就如同把香槟倒在还剩一点可乐的杯子里。\"",
	u8"发泡，时尚，火辣。"
	};

	init_render_format();
}
BartendMeun::~BartendMeun()
{
	RegionMgr::instance()->remove("button_prev");
	RegionMgr::instance()->remove("button_next");
	RegionMgr::instance()->remove("btc_meun");
}

void BartendMeun::on_render(SDL_Renderer* renderer)
{
	if (!valid) return;
	// 绘制背景框
	SDL_RenderCopy(renderer, ResMgr::instance()->find_texture("btc_meun"), nullptr, &rect);

	// 绘制欢迎界面
	if (idx == 0)
	{
		SDL_RenderCopy(renderer, ResMgr::instance()->find_texture("btc_icon"), nullptr, &rect_btc_icon);
		render_text_lines(renderer, { u8"欢迎回来，Jill? " , u8" ", u8" ", u8"使用导航栏", u8"浏览调酒配方" },
			{ rect_btc_icon.x + rect_btc_icon.w, rect_btc_icon.y }, 0.9, { 255,255,255,255 }, "IPix");
	}
	// 绘制配方
	else
	{
		render_formula(renderer);
	}

}

void BartendMeun::set_valid(bool flag)
{
	valid = flag;
	button_next.set_valid(valid);
	button_prev.set_valid(valid);
}

void BartendMeun::render_formula(SDL_Renderer* renderer)
{
	// 绘制配方
	int formula_idx = idx - 1;
	SDL_Point begin_point = { rect.x + 40, rect.y + 60 };
	render_textEx(renderer, { formula_list[formula_idx][0] },  { {255,255,255,255} }, begin_point);
	begin_point.y = rect.y + 120;

	for (int j = 0; j < formula_list_seg[formula_idx].size(); ++j)
	{
		auto [x, y] = render_textEx(renderer, formula_list_seg[formula_idx][j], formula_list_color[formula_idx][j], begin_point, "simhei", 0.87);
		begin_point.y += 45;
	}

	// 绘制页号
	//render_text_lines(renderer, formula_list[idx - 1], { rect.x + 50, rect.y + 50 }, 0.8);
	render_text(renderer, std::to_string(idx) + "/" + std::to_string(formula_list.size()), { rect.x + rect.w / 2, rect.y + rect.h - 40 });
}

void BartendMeun::init_render_format()
{
	for (int i = 0; i < formula_list.size(); ++i)
	{
		for (int j = 1; j < formula_list[i].size(); ++j)
		{
			auto& raw_str = formula_list[i][j];
			std::vector<std::string> post_str;
			std::vector<SDL_Color> post_color;
			static std::array<std::string, 5> key_text = { "Adelhyde" , "Bronson Extract" , "Powdered Delta" , "Flanergide" , "Karmotrine" };
			static std::array<SDL_Color, 5> value_color = { SDL_Color{200,40,70,255} , SDL_Color{225,200,60,255} , SDL_Color{135,165,225,255} , 
			SDL_Color{160,200,110,255} , SDL_Color{200,230,230,255} };

			int begin = 0;
			for (int k = 0; k < key_text.size(); ++k)
			{
				auto& key = key_text[k];
				auto& color = value_color[k];
				int pos = raw_str.find(key, begin);
				if (pos != std::string::npos)
				{
					int offset = 0;
					if (pos - 1 >= begin && raw_str[pos - 1] == ' ') offset = 2;
					else if(pos - strlen(u8"任选") >= begin) offset = strlen(u8"任选");
					post_str.push_back(raw_str.substr(begin, pos - begin - offset));
					post_color.push_back({ 255,255,255,255 });
					post_str.push_back(raw_str.substr(pos - offset, key.size() + offset));
					post_color.push_back(color);
					begin = pos + key.size();
				}
			}
			if (begin < raw_str.size())
			{
				post_str.push_back(raw_str.substr(begin, std::string::npos));
				post_color.push_back({ 255,255,255,255 });
			}
			formula_list_seg[i].push_back(std::move(post_str));
			formula_list_color[i].push_back(std::move(post_color));
		}
	}
}
