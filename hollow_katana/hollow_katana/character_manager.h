#pragma once
#include "character.h"

// ½ÇÉ«¹ÜÀíÆ÷
class CharacterManager
{
private:
	static CharacterManager* manager;
	Character* player = nullptr;
	Character* enemy = nullptr;

private:
	CharacterManager();
	~CharacterManager();

public:
	static CharacterManager* instance();
	void on_input(const ExMessage& msg);
	void on_update(float delta);
	void on_render();
	Character* get_player();
	Character* get_enemy();
};