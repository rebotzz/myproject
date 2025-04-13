#include "concrete_scene.h"

#include "scene_manager.h"
#include "resources_manager.h"
#include "cursor_manager.h"
#include "region_manager.h"
#include "goods_bundle.h"
#include "takeout_box.h"
#include "delivery_driver.h"
#include "microwave_oven.h"
#include "kits.h"
#include "game_system.h"


DayScene::DayScene()
{
	timer.set_wait_time(6);
	timer.set_one_shot(false);
	timer.set_on_timeout([this]
		{
			if (!RegionMgr::instance()->find("delivery_driver_2")->get_valid())
			{
				RegionMgr::instance()->find("delivery_driver_2")->set_valid(true);
			}
			else if (!RegionMgr::instance()->find("delivery_driver_3")->get_valid())
			{
				RegionMgr::instance()->find("delivery_driver_3")->set_valid(true);
			}
			else
			{
				timer.pause();
			}
		});
}

void DayScene::on_update(float delta)
{
	timer.on_update(delta);
	Scene::on_update(delta);
}

void DayScene::on_render(SDL_Renderer* renderer)
{
	static SDL_Texture* texture = ResMgr::instance()->find_texture("background");
	static SDL_Rect rect = { 0, 0 , 1280, 720 };
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
	Scene::on_render(renderer);
}

void DayScene::on_enter() 
{
	// 初始化可交互区域
	RegionMgr::instance()->add("delivery_driver_1", new DeliveryDriver(300, 145), 1);
	RegionMgr::instance()->add("delivery_driver_2", new DeliveryDriver(600, 145), 1);
	RegionMgr::instance()->add("delivery_driver_3", new DeliveryDriver(900, 145), 1);
	RegionMgr::instance()->find("delivery_driver_2")->set_valid(false);		// 防止刚开始外卖员全部到来
	RegionMgr::instance()->find("delivery_driver_3")->set_valid(false);

	RegionMgr::instance()->add("cola_bundle", new ColaBundle(260, 380), 2);
	RegionMgr::instance()->add("sprite_bundle", new SpriteBundle(400, 380), 2);
	RegionMgr::instance()->add("tb_bundle", new TbBundle(550, 420), 2);
	RegionMgr::instance()->add("mb_bundle", new MbBoxBundle(200, 520), 2);
	RegionMgr::instance()->add("bc_bundle", new BcBoxBundle(400, 520), 2);
	RegionMgr::instance()->add("rcp_boudle", new RcpBoxBundle(600, 520), 2);
	RegionMgr::instance()->add("mo_1", new MicrowaveOven(750, 400), 2);
	RegionMgr::instance()->add("mo_2", new MicrowaveOven(1000, 400), 3);
	RegionMgr::instance()->add("takeout_box_1", new TakeoutBox(800, 550), 4);
	RegionMgr::instance()->add("takeout_box_2", new TakeoutBox(900, 550), 4);
	RegionMgr::instance()->add("takeout_box_3", new TakeoutBox(1000, 550), 4);
	RegionMgr::instance()->add("takeout_box_4", new TakeoutBox(1100, 550), 4);
	RegionMgr::instance()->add("takeout_box_5", new TakeoutBox(1200, 550), 4);

	timer.restart();
}
void DayScene::on_exit() 
{
	RegionMgr::instance()->remove("delivery_driver_1");
	RegionMgr::instance()->remove("delivery_driver_2");
	RegionMgr::instance()->remove("delivery_driver_3");
						   
	RegionMgr::instance()->remove("cola_bundle");
	RegionMgr::instance()->remove("sprite_bundle");
	RegionMgr::instance()->remove("tb_bundle");
	RegionMgr::instance()->remove("mb_bundle");
	RegionMgr::instance()->remove("bc_bundle");
	RegionMgr::instance()->remove("rcp_boudle");
	RegionMgr::instance()->remove("mo_1"); 
	RegionMgr::instance()->remove("mo_2"); 
	RegionMgr::instance()->remove("takeout_box_1");
	RegionMgr::instance()->remove("takeout_box_2");
	RegionMgr::instance()->remove("takeout_box_3");
	RegionMgr::instance()->remove("takeout_box_4");
	RegionMgr::instance()->remove("takeout_box_5");
}


NightScene::NightScene()
{

}
void NightScene::on_update(float delta)
{

}
void NightScene::on_render(SDL_Renderer* renderer)
{

}
void NightScene::on_enter() {}
void NightScene::on_exit() {}



TransitionScene::TransitionScene()
{
	timer.set_one_shot(true);
	timer.set_wait_time(3.0);
	timer.set_on_timeout([this]
		{
			SceneMgr::instance()->switch_scene(next_scene);
		});
}

void TransitionScene::on_input(const SDL_Event& event) 
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	case SDL_KEYDOWN:
		counter++;
		if (counter >= 2)	// 双击跳过
		{
			SceneMgr::instance()->switch_scene(next_scene);
		}
		break;
	}
}
void TransitionScene::on_update(float delta) 
{
	timer.on_update(delta);
}
void TransitionScene::on_render(SDL_Renderer* renderer) 
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	static SDL_Point center_point = { 1280 / 2, 720 / 2 };
	static SDL_Color color = { 255,255, 255, 255 };
	render_text(renderer, text, center_point, color);
}
void TransitionScene::on_enter() 
{
	timer.restart();
	counter = 0;
}
void TransitionScene::set_wait_time(float delta) 
{
	timer.set_wait_time(delta);
}
void TransitionScene::set_text(const std::string& val)
{
	text = val;
}
void TransitionScene::set_next_scene(const std::string& id)
{
	next_scene = id;
}