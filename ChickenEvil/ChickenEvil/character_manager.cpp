#include "character_manager.h"
#include "particle_manager.h"


CharacterManager* CharacterManager::manager = nullptr;
CharacterManager* CharacterManager::instance()
{
	if (manager == nullptr)
	{
		manager = new CharacterManager;
	}
	return manager;
}

static std::shared_ptr<Chicken> spawn_chicken();

CharacterManager::CharacterManager()
{
	timer_spawn_enemy.set_one_shot(false);
	timer_spawn_enemy.set_wait_time(0.25f);
	timer_spawn_enemy.set_on_timeout([&]()
		{
			static int counter = 0;
			static int mod = 3;
			if (++counter % 150 == 0)
			{
				if (mod < 6) mod++;					// 0.25*150 s后增加刷新速度,直到2倍数
				else
				{
					// 超过2倍数后，刷新速度随机波动
					if (score <= 800) mod += (rand() % 2 ? 3 : -3);
					else if (score <= 1500) mod += (rand() % 2 ? 4 : -4);
					else mod += (rand() % 2 ? 5 : -5);
				}
				if (score <= 700)					// 6~10
				{
					mod = std::max(6, mod);
					mod = std::min(10, mod);
				}
				else if (score <= 1500)				// 10~14
				{
					mod = std::max(10, mod);
					mod = std::min(14, mod);
				}
				else if(score <= 2500)				// 14~18
				{
					mod = std::max(14, mod);
					mod = std::min(18, mod);
				}
				else
				{
					mod = std::max(18, mod);
					mod = std::min(22, mod);
				}
			}

			int rand_num = rand() % mod;
			for (int i = 0; i < rand_num; ++i)
				enemy_list.push_back(spawn_chicken());
		});

	turrent_list.push_back(std::shared_ptr<Turrent>(new Turrent({ WINDOW_W / 2, 600 })));
	//turrent_list.push_back(std::shared_ptr<Turrent>(new TurrentAuto({ WINDOW_W / 2 - 550, 600 }, -45.0, 30.0, true)));
	//turrent_list.push_back(std::shared_ptr<Turrent>(new TurrentAuto({ WINDOW_W / 2 + 550, 600 }, -135.0, 30.0, false)));
	//turrent_list.push_back(std::shared_ptr<Turrent>(new Turrent({ WINDOW_W / 2 + 300, 600 })));
	//turrent_list.push_back(std::shared_ptr<Turrent>(new Turrent({ WINDOW_W / 2 - 300, 600 })));
}

void CharacterManager::on_input(SDL_Event* event)
{
	if (home_hp <= 0)
	{
		return;
	}

	for (auto& turrent : turrent_list)
		turrent->on_input(event);
}

void CharacterManager::on_update(float delta)
{
	// 游戏结束
	if (home_hp <= 0)
	{
		return;
	}

	if (score >= 200 && turrent_list.size() < 2)
	{
		turrent_list.push_back(std::shared_ptr<Turrent>(new Turrent({ WINDOW_W / 2 + 300, 600 })));
	}
	else if (score >= 600 && turrent_list.size() < 3)
	{
		turrent_list.push_back(std::shared_ptr<Turrent>(new Turrent({ WINDOW_W / 2 - 300, 600 })));
	}
	else if (score >= 1500 && turrent_list.size() < 4)
	{
		turrent_list.push_back(std::shared_ptr<Turrent>(new TurrentAuto({ WINDOW_W / 2 - 550, 600 }, -45.0, 30.0, true)));
	}
	else if (score >= 3000 && turrent_list.size() < 5)
	{
		turrent_list.push_back(std::shared_ptr<Turrent>(new TurrentAuto({ WINDOW_W / 2 + 550, 600 }, -135.0, 30.0, false)));
	}

	// 敌人刷新
	timer_spawn_enemy.on_update(delta);
	// 敌人更新
	for (auto& enemy : enemy_list)
	{
		enemy->on_update(delta);

		// 射杀，加分
		if (enemy->is_shooting_death())
		{
			score++;
		}
		// 越界，扣血
		if (enemy->is_alive() && enemy->get_position().y > (double)WINDOW_H + 50.0f)
		{
			enemy->dead();
			home_hp--;
			Mix_PlayChannel(-1, ResourcesManager::instance()->find_audio_chunk("hurt"), 0);
		}
	}
	enemy_list.erase(std::remove_if(enemy_list.begin(), enemy_list.end(), 
		[](const std::shared_ptr<Chicken>& chicken)
		{
			return !chicken->is_alive();
		}), enemy_list.end());

	// 炮塔更新
	for (auto& turrent : turrent_list)
	{
		turrent->on_update(delta);
	}

	// 粒子特效更新
	ParticleManager::instance()->on_update(delta);
}

void CharacterManager::on_render(SDL_Renderer* renderer, const Camera& camera)
{
	// 炮塔
	for (auto& turrent : turrent_list)
	{
		turrent->render_bottom(renderer, camera);
	}

	// 敌人
	for (auto& enemy : enemy_list)
	{
		enemy->on_render(renderer, camera);
	}

	// 炮塔
	for (auto& turrent : turrent_list)
	{
		turrent->on_render(renderer, camera);
	}

	// 粒子
	ParticleManager::instance()->on_render(renderer, camera);

	if (home_hp <= 0)
	{
		std::string str = "Game Over! Score: " + std::to_string(score);
		SDL_Surface* suf_text = TTF_RenderText_Blended(ResourcesManager::instance()->find_font("IPix"),
			str.c_str(), { 0, 0, 0, 255 });
		SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
		SDL_Rect rect_msg_box, rect_text;
		rect_msg_box = { WINDOW_W / 2 - 250, WINDOW_H / 2 - 100, 500, 200 };
		rect_text = { rect_msg_box.x + rect_msg_box.w / 2 - suf_text->w / 2, 
			rect_msg_box.y + rect_msg_box.h / 2 - suf_text->h / 2,  suf_text->w, suf_text->h };
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
		SDL_RenderFillRect(renderer, &rect_msg_box);
		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);
		SDL_FreeSurface(suf_text);
		SDL_DestroyTexture(tex_text);
	}
}

static std::shared_ptr<Chicken> spawn_chicken()
{
	// 随机种类
	std::shared_ptr<Chicken> chicken = nullptr;
	int rand_num = rand() % 100;
	if (rand_num <= 30)
	{
		chicken = std::shared_ptr<Chicken>(new ChickenSlow);
	}
	else if (rand_num <= 80)
	{
		chicken = std::shared_ptr<Chicken>(new ChickenMedium);
	}
	else
	{
		chicken = std::shared_ptr<Chicken>(new ChickenFast);
	}

	return chicken;
}

void CharacterManager::render_status(SDL_Renderer* renderer, const Camera& camera)
{
	// 渲染生命值
	static SDL_Texture* tex_img = ResourcesManager::instance()->find_image("heart");
	static SDL_Rect rect_hp = {30, 30, 0, 0};
	SDL_QueryTexture(tex_img, nullptr, nullptr, &rect_hp.w, &rect_hp.h);

	for (int i = 0; i < home_hp; ++i)
	{
		rect_hp.x = 30 + i * 40;
		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_hp);
	}

	// 渲染得分
	static TTF_Font* font = ResourcesManager::instance()->find_font("IPix");
	static SDL_Rect rect_score = { WINDOW_W * 0.8, 30, 0, 0 };
	std::string str = "Score: " + std::to_string(score);
	SDL_Surface* suf_text = TTF_RenderUTF8_Solid(font, str.c_str(), { 255,255,255,255 });
	rect_score.w = suf_text->w, rect_score.h = suf_text->h;
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
	SDL_RenderCopy(renderer, tex_text, nullptr, &rect_score);
	SDL_FreeSurface(suf_text);
	SDL_DestroyTexture(tex_text);
}