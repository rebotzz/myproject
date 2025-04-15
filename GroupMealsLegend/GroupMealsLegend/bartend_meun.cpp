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

	formula_list[0] = u8"Sugar Rush - $150\nSugar Rush����2 Adelhyde��1 Powdered Delta��\n��ѡKarmotrinei���ƶ��ɡ�\n\n\"�����嵭��ˮ����ζ�����ܸ���Ů�Ի�����Ʒ��\"\n��ζ��Ů�Ի�����⡣";
	formula_list[1] = u8"Fluffy Dream - $170\nFluffy Dream����3 Adelhyde��3 Powdered Delta��\n��ѡKarmotrine�»������ƶ��ɡ�\n����������ơ���ť����ȴ���ҡ�ƺ���ʼ���һζ�����\n\n\"һ���ھ�����ȡ�������ͷ���ٶ�ȾͿ��ܻᵼ��˯��\nͷ��\"\n��ζ��Ů�Ի����º͡�";
	formula_list[2] = u8"Moonblast - $180\nMoonblast����6 Adelhyde��1 Powdered Delta\n1 Flanergide��2 Karmotrine�ӱ������Ͷ��ɡ�\n\n\"����ÿ���¶���һ��ʱ���ܿ�������������ǿ�Ӵ���û\n���κι�ϵ��\"\n��ζ��Ů�Ի�����⡣";
	formula_list[3] = u8"Cobalt Velvet - $280\nCobalt Velvet����2 Adelhyde��3 Flanergide��\n5 Karmotrine�ӱ������ƶ��ɡ�\n\n\"����ͬ�����ĵ��ڻ�ʣһ����ֵı����\"\n���ݣ�ʱ�У�������";
	
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
		render_text(renderer, u8"��ӭ������Jill? Player.", { rect_btc_icon.x + rect_btc_icon.w, rect_btc_icon.y,
			200, 100 }, { 255,255,255,255 });
		render_text(renderer, u8"ʹ�õ�����,��������䷽", { rect_btc_icon.x + rect_btc_icon.w, rect_btc_icon.y + 200,
		200, 100 }, { 255,255,255,255 });
		render_text(renderer, u8"��������䷽", { rect_btc_icon.x + rect_btc_icon.w, rect_btc_icon.y + 260,
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