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

EffectTimeRun::EffectTimeRun()
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
EffectTimeRun::~EffectTimeRun()
{}

void EffectTimeRun::add_image(IMAGE& image)
{
	ResourcesManager::bend_image(&image, &frame, base, 0.55f);

	animation_vfx.add_frame(&frame, 1);
}