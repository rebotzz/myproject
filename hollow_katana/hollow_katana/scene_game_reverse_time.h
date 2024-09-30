#pragma once
#include <memory>
#include "scene.h"
#include "reverse_time_manager.h"
#include "audio_manager.h"
#include "particle_manager.h"
#include "effect.h"

class SceneGameReverseTime : public Scene
{
private:
	Timer timer_text;

public:
	SceneGameReverseTime()
	{
		timer_text.set_one_shot(false);
		timer_text.set_wait_time(0.49f);
		timer_text.set_on_timeout([&]
			{
				std::shared_ptr<EffectText> text = std::make_shared<EffectText>(_T("��[�����]����"), 0.5f);
				text->set_position({ (float)getwidth() / 2 + 430, (float)getheight() / 2 + 280 });
				text->set_enable_background(false);
				ParticleManager::instance()->register_particle(text);
			});
	}
	virtual void on_enter() override 
	{
		// ֻ�ý��뿪������ʱ�����������,���������ת��һ����Ϸ����
		ReverseTimeManager::instance()->reverse_time();
		AudioManager::instance()->play_audio_ex(_T("reverse_time"), true);
	}

	virtual void on_input(const ExMessage& msg) override 
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			ReverseTimeManager::instance()->skip();
			break;
		}
	}

	virtual void on_update(float delta) override 
	{
		ReverseTimeManager::instance()->on_update(delta);
		timer_text.on_update(delta);
		ParticleManager::instance()->on_update(delta);
	}
	virtual void on_render() override 
	{
		ReverseTimeManager::instance()->on_render();
		ParticleManager::instance()->on_render();
	}
	virtual void on_exit() override 
	{
		ReverseTimeManager::instance()->set_enable(false);
		AudioManager::instance()->stop_audio_ex(_T("reverse_time"));
	}
};