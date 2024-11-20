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
	timer_spawn_enemy.set_wait_time(0.3f);
	timer_spawn_enemy.set_on_timeout([&]()
		{
			int rand_num = rand() % 3;
			for (int i = 0; i < rand_num; ++i)
				enemy_list.push_back(spawn_chicken());
		});

	turrent_list.push_back(std::shared_ptr<Turrent>(new Turrent({ WINDOW_W / 2, 600 })));
	//turrent_list.push_back(std::shared_ptr<Turrent>(new Turrent({ WINDOW_W / 2 - 300, 600 })));
	//turrent_list.push_back(std::shared_ptr<Turrent>(new Turrent({ WINDOW_W / 2 + 300, 600 })));
}

void CharacterManager::on_input(SDL_Event* event)
{
	for (auto& turrent : turrent_list)
		turrent->on_input(event);
}

void CharacterManager::on_update(float delta)
{
	// 游戏结束
	if (home_hp <= 0)
	{
		Mix_PlayMusic(ResourcesManager::instance()->find_audio_music("loss"), 0);
		SDL_Log("game over!\n");
		system("pause");
		exit(0);
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
	static SDL_Texture* tex_img = nullptr;
	static SDL_Rect rect_hp = {30, 30, 0, 0};
	if (nullptr == tex_img)
	{
		SDL_Surface* suf_img = ResourcesManager::instance()->find_image("heart");
		tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);
		rect_hp.w = suf_img->w;
		rect_hp.h = suf_img->h;
	}

	for (int i = 0; i < home_hp; ++i)
	{
		rect_hp.x = 30 + i * 40;
		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_hp);
	}

	// 渲染得分
	static TTF_Font* font = ResourcesManager::instance()->find_font("IPix");
	static SDL_Rect rect_score = { WINDOW_W * 0.8, 30, 0, 0 };
	std::string str = "Score: " + std::to_string(score);
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, str.c_str(), { 255,255,255,255 });
	rect_score.w = suf_text->w, rect_score.h = suf_text->h;
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);
	SDL_RenderCopy(renderer, tex_text, nullptr, &rect_score);
	SDL_FreeSurface(suf_text);
	SDL_DestroyTexture(tex_text);
}