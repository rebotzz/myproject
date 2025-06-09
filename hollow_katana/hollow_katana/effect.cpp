#include "effect.h"
#include "resources_manager.h"


EffectHit::EffectHit(bool is_left)
{
	// 加载动画资源
	animation_vfx.add_frame(ResourcesManager::instance()->find_atlas(
		is_left ? "particle_vfx_hit_left" : "particle_vfx_hit_right"));
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.set_interval(0.03f);
}

EffectHurt::EffectHurt(bool is_left)
{
	// 加载动画资源
	animation_vfx.add_frame(ResourcesManager::instance()->find_atlas(
		is_left ? "particle_vfx_hurt_red_left" : "particle_vfx_hurt_red_right"));
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.set_interval(0.03f);
}



EffectBlend::EffectBlend()
{
	// 加载动画资源
	animation_vfx.set_achor_mode(Animation::AchorMode::BottomCentered);
	animation_vfx.set_interval(0.15f);
	animation_vfx.set_on_finished([&]
		{
			is_valid = false;
			animation_vfx.clear();
		});

	base = ResourcesManager::instance()->find_image("effect_mixed_blue_1");
}


void EffectBlend::add_image(IMAGE& image)
{
	ResourcesManager::bend_image(&image, &frame, base, blend_ratio);
	animation_vfx.add_frame(&frame, 1);
}

EffectBulletTime::EffectBulletTime()
{
	animation_vfx.set_interval(0.3f);
	base = ResourcesManager::instance()->find_image("effect_mixed_blue_1");
	blend_ratio = 0.55f;
}

EffectRoll::EffectRoll()
{
	base = ResourcesManager::instance()->find_image("effect_mixed_blue_2");
	animation_vfx.set_interval(0.07f);
	blend_ratio = 0.35f;
}


EffectText::EffectText(LPCTSTR text, float delta, COLORREF color)
{
	this->text = text;

	text_color = color;
	timer.set_one_shot(true);
	timer.set_wait_time(delta);
	timer.set_on_timeout([&]
		{
			is_valid = false;
		});

	background = ResourcesManager::instance()->find_image("test_background_black");
}
void EffectText::on_update(float delta)
{
	timer.on_update(delta);
}
void EffectText::on_render()
{
	// 居中绘制
	if (enable_background)
	{
		putimage_alpha((int)position.x - background->getwidth() / 2,
			(int)position.y - background->getheight() / 2, background);
	}
	settextcolor(text_color);
	outtextxy((int)position.x - textwidth(text) / 2 + offset_x, (int)position.y - textheight(text) / 2 + offset_y, text);
}

EffectJump::EffectJump()
{
	// 加载动画资源
	animation_vfx.set_interval(0.05f);
	animation_vfx.set_achor_mode(Animation::AchorMode::BottomCentered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_image("player_vfx_jump"), 5);
}

EffectLand::EffectLand()
{
	// 加载动画资源
	animation_vfx.set_interval(0.1f);
	animation_vfx.set_achor_mode(Animation::AchorMode::BottomCentered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_image("player_vfx_land"), 2);
}

EffectLeaves::EffectLeaves()
{
	// 加载动画资源
	animation_vfx.set_interval(0.075f);
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_atlas("particle_vfx_leaves"));
}

EffectElectric::EffectElectric()
{
	// 加载动画资源
	animation_vfx.set_interval(0.075f);
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_atlas("particle_vfx_electric"));
}

EffectElectricAxis::EffectElectricAxis(bool is_left)
{
	// 加载动画资源
	animation_vfx.set_interval(0.04f);
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_atlas(
		is_left ? "particle_vfx_electric_left" : "particle_vfx_electric_right"));
}

EffectFire::EffectFire(bool is_left)
{
	// 加载动画资源
	animation_vfx.set_interval(0.075f);
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_atlas(is_left ? 
		"particle_vfx_fire_left" : "particle_vfx_fire_right"));
}

EffectDashLine::EffectDashLine(bool is_left)
{
	// 加载动画资源
	animation_vfx.set_interval(0.02f);
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_image(
		is_left ? "dragon_vfx_dash_line_left" : "dragon_vfx_dash_line_right"), 11);
}

EffectSwordHit::EffectSwordHit(bool is_left)
{
	// 加载动画资源
	animation_vfx.set_interval(0.05f);
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_image(
		is_left ? "sword_hit_left" : "sword_hit_right"), 6);
}

EffectUiChoose::EffectUiChoose(bool is_left)
{
	// 加载动画资源
	animation_vfx.set_interval(0.05f);
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_image(
		is_left ? "sword_hit_left" : "sword_hit_right"), 6);
}

EffectHurt2::EffectHurt2()
{
	animation_vfx.set_interval(0.04f);
	animation_vfx.set_achor_mode(Animation::AchorMode::Centered);
	animation_vfx.add_frame(ResourcesManager::instance()->find_atlas("particle_vfx_hurt"));
}

