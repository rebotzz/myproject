#pragma once
#include "turrent.h"
#include "chicken.h"

class CharacterManager
{
private:
	std::vector<std::shared_ptr<Chicken>> enemy_list;
	std::vector<std::shared_ptr<Turrent>> turrent_list;
	Timer timer_spawn_enemy;
	int home_hp = 10;
	int score = 0;

private:
	static CharacterManager* manager;
private:
	CharacterManager();
	CharacterManager(const CharacterManager&) = delete;

public:
	static CharacterManager* instance();
	void on_input(SDL_Event* event);
	void on_update(float delta);
	void on_render(const Camera& camera) const;
	void render_status(const Camera& camera) const;
	bool is_alive() const { return home_hp > 0; }
	int get_score() const { return score; }
};