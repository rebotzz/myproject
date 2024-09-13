#include <memory>
#include "scene.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "particle_manager.h"
#include "reverse_time_manager.h"
#include "audio_manager.h"
#include "bullet_time_manager.h"




void GameScene::on_enter()
{
	CharacterManager::instance()->set_enable(true);
	auto* player = CharacterManager::instance()->get_player();
	player->reset();
	player->switch_state("idle");

	ReverseTimeManager::instance()->set_enable(true);
}

void GameScene::on_input(const ExMessage& msg)
{
	CharacterManager::instance()->on_input(msg);
}

void GameScene::on_update(float delta)
{
	// 更新玩家和粒子状态
	CharacterManager::instance()->on_update(delta);
	ParticleManager::instance()->on_update(delta);

	// 记录历史画面,用于倒放
	if (CharacterManager::instance()->get_player()->get_hp() <= 0)
		ReverseTimeManager::instance()->set_record(false);
	else
		ReverseTimeManager::instance()->set_record(true);
	ReverseTimeManager::instance()->on_update(delta);

	// 子弹时间暂停bgm
	if (BulletTimeManager::instance()->is_enable())
		AudioManager::instance()->pause_audio_ex(bgm);
	else
		AudioManager::instance()->resume_audio_ex(bgm);
}

void GameScene::on_render()
{
	render_background();
	CharacterManager::instance()->on_render();
	ParticleManager::instance()->on_render();
}

void GameScene::on_exit()
{
	CharacterManager::instance()->set_enable(false);
	ReverseTimeManager::instance()->set_enable(false);

	AudioManager::instance()->stop_audio_ex(bgm);
	AudioManager::instance()->play_audio_ex(_T("reverse_time"), true);	// debug:为了确保成功播放,所以两次
}

void GameScene::render_background()
{
	if (nullptr == background)
		throw std::invalid_argument("background is nullptr.");

	static Rect rect_dst;
	rect_dst.x = (getwidth() - rect_dst.w) / 2;
	rect_dst.y = (getheight() - rect_dst.h) / 2;
	rect_dst.w = background->getwidth(), rect_dst.h = background->getheight();

	setbkcolor(RGB(0, 0, 0));
	putimage_alpha_ex(background, &rect_dst);
}

