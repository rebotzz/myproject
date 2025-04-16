#include "bartend_system.h"

#include "resources_manager.h"
#include "region_manager.h"

BartendSystem* BartendSystem::manager = nullptr;
BartendSystem* BartendSystem::instance()
{
	if (!manager)
	{
		manager = new BartendSystem;
	}
	return manager;
}


BartendSystem::BartendSystem()
	:button_redo(815, 565), button_modulate(1115, 565)
{
	button_redo.set_describe(u8"重做");
	button_redo.set_texture(ResMgr::instance()->find_texture("button_2"));
	button_redo.set_on_click([this]()
		{
			reset();
		});
	button_modulate.set_describe(u8"调制");
	button_modulate.set_texture(ResMgr::instance()->find_texture("button_3"));
	button_modulate.set_on_click([this]()
		{
			modulate();
		});

	btc_bottom.set_render_callback([](SDL_Renderer* renderer)
		{
			// 绘制背景框
			static SDL_Texture* tex_bartend = ResMgr::instance()->find_texture("bartending");
			static SDL_Rect rect_bt = { 700,155,583, 470 };
			SDL_RenderCopy(renderer, tex_bartend, nullptr, &rect_bt);
		});


	RegionMgr::instance()->add("btc_bottom", &btc_bottom, 0);
	RegionMgr::instance()->add("Adelhyde", &adelhyde, 1);
	RegionMgr::instance()->add("BronsonExt", &bronsonext, 1);
	RegionMgr::instance()->add("PwdDelta", &pwddelta, 1);
	RegionMgr::instance()->add("Flanergide", &flanergide, 1);
	RegionMgr::instance()->add("Karmotrine", &karmotrine, 1);
	RegionMgr::instance()->add("Ice", &ice, 1);
	RegionMgr::instance()->add("Ageing", &ageing, 1);
	RegionMgr::instance()->add("BartendBottle", &bartendbottle, 1);

	RegionMgr::instance()->add("ButtonRedo", &button_redo, 1);
	RegionMgr::instance()->add("ButtonModulate", &button_modulate, 1);

	btc_bottom.set_valid(false);
	adelhyde.set_valid(false);
	bronsonext.set_valid(false);
	pwddelta.set_valid(false);
	flanergide.set_valid(false);
	karmotrine.set_valid(false);
	ice.set_valid(false);
	ageing.set_valid(false);
	bartendbottle.set_valid(false);
	button_redo.set_valid(false);
	button_modulate.set_valid(false);

	timer_button.set_one_shot(false);
	timer_button.set_wait_time(0.1);
	timer_button.set_on_timeout([this]()
		{
			switch (status)
			{
			case Status::Init: button_modulate.set_describe(u8"调制"); break;
			case Status::Doing: button_modulate.set_describe(u8"停止"); break;
			case Status::Done: button_modulate.set_describe(u8"上酒"); break;
			}
		});
}

void BartendSystem::open()
{
	if (valid == false)
	{
		valid = true;

		btc_bottom.set_valid(true);
		adelhyde.set_valid(true);
		bronsonext.set_valid(true);
		pwddelta.set_valid(true);
		flanergide.set_valid(true);
		karmotrine.set_valid(true);
		ice.set_valid(true);
		ageing.set_valid(true);
		bartendbottle.set_valid(true);
		button_redo.set_valid(true);
		button_modulate.set_valid(true);
	}
}

void BartendSystem::close()
{
	if (valid == true)
	{
		valid = false;

		btc_bottom.set_valid(false);
		adelhyde.set_valid(false);
		bronsonext.set_valid(false);
		pwddelta.set_valid(false);
		flanergide.set_valid(false);
		karmotrine.set_valid(false);
		ice.set_valid(false);
		ageing.set_valid(false);
		bartendbottle.set_valid(false);
		button_redo.set_valid(false);
		button_modulate.set_valid(false);
	}
}

void BartendSystem::on_update(float delta)
{
	timer_button.on_update(delta);
}

void BartendSystem::reset()
{
	adelhyde.set_count(0);
	bronsonext.set_count(0);
	pwddelta.set_count(0);
	flanergide.set_count(0);
	karmotrine.set_count(0);

	bartendbottle.reset();
	bartendmeun.reset();
	status = Status::Init;
}
void BartendSystem::modulate()
{
	if (Status::Init == status) status = Status::Doing;
	else if (Status::Doing == status) status = Status::Done;

	bartendbottle.modulate();
}
void BartendSystem::enable_meun(bool flag)
{
	bartendmeun.set_valid(flag);
}
