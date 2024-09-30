#include "scene_game.h"

#include <memory>
#include "character_manager.h"
#include "particle_manager.h"
#include "reverse_time_manager.h"
#include "audio_manager.h"
#include "bullet_time_manager.h"
#include "scene_manager.h"
#include "effect.h"

GameScene::GameScene(IMAGE* background, const std::wstring bgm)
{
	this->background = background;
	this->bgm = bgm;

	timer_player_dead_text;
	timer_player_dead_text.set_one_shot(false);
	timer_player_dead_text.set_wait_time(0.49f);
	timer_player_dead_text.set_on_timeout([]
		{
			// 死亡文本提示
			std::shared_ptr<EffectText> text_1 = std::make_shared<EffectText>(_T("不对......这样不行"),
				0.5f, RGB(0, 255, 255));
			std::shared_ptr<EffectText> text_2 = std::make_shared<EffectText>(_T("按[R]回溯时间"),
				0.5f, RGB(0, 255, 255));
			text_1->set_position({ (float)getwidth() / 2, (float)getheight() / 2 - 20 });
			text_1->set_text_offset(0, -20);
			text_2->set_position({ (float)getwidth() / 2, (float)getheight() / 2 + 20 });
			text_2->set_enable_background(false);
			ParticleManager::instance()->register_particle(text_1);
			ParticleManager::instance()->register_particle(text_2);
		});

	timer_cd.set_one_shot(true);
	timer_cd.set_wait_time(2.0f);
	timer_cd.set_on_timeout([&]()
		{
			can_reverse_time = true;
		});
}

void GameScene::on_enter()
{
	CharacterManager::instance()->set_enable(true);
	auto* player = CharacterManager::instance()->get_player();
	player->reset();
	player->switch_state("idle");

	can_reverse_time = false;
	is_reverse_time = false;
	timer_cd.restart();

	ReverseTimeManager::instance()->set_enable(true);
	AudioManager::instance()->play_audio_ex(bgm, true);
}

void GameScene::on_input(const ExMessage& msg)
{
	CharacterManager::instance()->on_input(msg);

	static const int VK_R = 0x52;
	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case VK_R:
		case VK_SPACE:
			is_reverse_key_down = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case VK_R:
		case VK_SPACE:
			is_reverse_key_down = false;
			break;
		}
		break;
	}

	// 如果玩家死亡
	if (CharacterManager::instance()->get_player()->get_hp() <= 0
		&& can_reverse_time && is_reverse_key_down)
		is_reverse_time = true;
}

void GameScene::on_update(float delta)
{
	// 更新玩家和粒子状态
	CharacterManager::instance()->on_update(delta);
	ParticleManager::instance()->on_update(delta);

	// 记录历史画面,用于倒放
	if (CharacterManager::instance()->get_player()->get_hp() <= 0
		|| CharacterManager::instance()->get_enemy()->get_hp() <= 0)
		ReverseTimeManager::instance()->set_record(false);
	else
		ReverseTimeManager::instance()->set_record(true);
	ReverseTimeManager::instance()->on_update(delta);

	// 子弹时间暂停bgm
	if (BulletTimeManager::instance()->is_enable())
		AudioManager::instance()->pause_audio_ex(bgm);
	else
		AudioManager::instance()->resume_audio_ex(bgm);

	// 如果玩家死亡
	if (CharacterManager::instance()->get_player()->get_hp() <= 0)
	{
		timer_cd.on_update(delta);
		timer_player_dead_text.on_update(delta);
		if(is_reverse_time)
			SceneManager::instance()->switch_scene("game_reverse_time");
	}
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
	if (CharacterManager::instance()->get_player()->get_hp() <= 0)
	{
		AudioManager::instance()->play_audio_ex(_T("reverse_time"), true);		// debug:为了确保成功播放,所以两次

#ifdef REVERSE_TIME_VERSION_2
		ReverseTimeManager::instance()->set_background(background);
#endif
	}
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

void GameScene::clear_record()
{
	ReverseTimeManager::instance()->set_enable(false);
	ReverseTimeManager::instance()->clear();
}