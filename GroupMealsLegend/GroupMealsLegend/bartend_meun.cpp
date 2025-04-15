#include "bartend_meun.h"
#include "region_manager.h"
#include "resources_manager.h"
#include "kits.h"

BartendMeun::BartendMeun()
{
	SDL_Rect rect_prev = { rect_meun.x + 30, rect_meun.y + rect_meun.h - 80 , 0 ,0};
	SDL_QueryTexture(ResMgr::instance()->find_texture("button_prev"), nullptr, nullptr, &rect_prev.w, &rect_prev.h);
	button_prev.set_rect(rect_prev);
	button_prev.set_texture(ResMgr::instance()->find_texture("button_prev"));
	button_prev.set_on_click([this]()
		{
			idx--;
			if (idx < 0) idx = 0;
		});

	SDL_Rect rect_next = { rect_meun.x + rect_meun.w - 100, rect_meun.y + rect_meun.h - 80 , 0 ,0 };
	SDL_QueryTexture(ResMgr::instance()->find_texture("button_next"), nullptr, nullptr, &rect_prev.w, &rect_prev.h);
	button_prev.set_rect(rect_next);
	button_next.set_texture(ResMgr::instance()->find_texture("button_next"));
	button_next.set_on_click([this]()
		{
			idx++;
			if (idx >= formula_list.size()) idx = formula_list.size() - 1;
		});

	RegionMgr::instance()->add("button_prev", &button_prev, 0);
	RegionMgr::instance()->add("button_next", &button_next, 0);

	formula_list[0] = u8"Sugar Rush - $150\nSugar Rush是由2 Adelhyde、1 Powdered Delta和\n任选Karmotrinei调制而成。\n\n\"甘甜，清淡，水果风味，不能更加女性化的饮品。\"\n甜味，女性化，惬意。";
	formula_list[1] = u8"Fluffy Dream - $170\nFluffy Dream是由3 Adelhyde、3 Powdered Delta和\n任选Karmotrine陈化，调制而成。\n（点击“调制”按钮后请等待至摇酒壶开始剧烈晃动。）\n\n\"一两口就足以取悦你的舌头，再多喝就可能会导致睡过\n头。\"\n酸味，女性化，温和。";
	formula_list[2] = u8"Moonblast - $180\nMoonblast是由6 Adelhyde、1 Powdered Delta\n1 Flanergide和2 Karmotrine加冰，调和而成。\n\n\"与你每个月都有一周时间能看到的那座月球强子大炮没\n有任何关系。\"\n甜味，女性化，惬意。";
	formula_list[3] = u8"Cobalt Velvet - $280\nCobalt Velvet是由2 Adelhyde、3 Flanergide和\n5 Karmotrine加冰，调制而成。\n\n\"就如同把香槟倒在还剩一点可乐的杯子里。\"\n发泡，时尚，火辣。";
	
}
BartendMeun::~BartendMeun()
{
	RegionMgr::instance()->remove("button_prev");
	RegionMgr::instance()->remove("button_next");
}

void BartendMeun::on_input(const SDL_Event& event)
{
	if (status == Status::Init)
		status = Status::Describe;
}
void BartendMeun::on_update(float delta)
{

}
void BartendMeun::on_render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, ResMgr::instance()->find_texture("btc_meun"), nullptr, &rect_meun);

	switch (status)
	{
	case Status::Init:
		SDL_Rect rect_btc_icon = { rect_meun.x + 100, rect_meun.y + 100, 0, 0 };
		SDL_QueryTexture(ResMgr::instance()->find_texture("btc_icon"), nullptr, nullptr, &rect_btc_icon.w, &rect_btc_icon.h);
		SDL_RenderCopy(renderer, ResMgr::instance()->find_texture("btc_icon"), nullptr, &rect_btc_icon);
		render_text(renderer, u8"欢迎回来，Jill? Player.", { rect_btc_icon.x + rect_btc_icon.w, rect_btc_icon.y,
			200, 100 }, { 255,255,255,255 });
		render_text(renderer, u8"使用导航栏,浏览调酒配方", { rect_btc_icon.x + rect_btc_icon.w, rect_btc_icon.y + 200,
		200, 100 }, { 255,255,255,255 });
		render_text(renderer, u8"浏览调酒配方", { rect_btc_icon.x + rect_btc_icon.w, rect_btc_icon.y + 260,
		200, 100 }, { 255,255,255,255 });
		break;

	case Status::Describe:
		render_text(renderer, formula_list[idx], rect_meun, { 255,255,255,255 });
		break;
	}
}

void BartendMeun::set_valid(bool flag)
{
	valid = flag;
	button_next.set_valid(valid);
	button_prev.set_valid(valid);
}