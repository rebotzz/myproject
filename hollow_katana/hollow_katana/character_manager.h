#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "character.h"

// ��ɫ������
class CharacterManager
{
private:
	static CharacterManager* manager;
	std::shared_ptr<Character> player = nullptr;
	std::unordered_map<std::string, std::shared_ptr<Character>> enemy_list;
	bool is_enable = false;

	// debug
	//std::shared_ptr<Character> player_2 = nullptr;	// ��Ϊ�ٿص���

	// todo: 
	// ���ӵ����б�, ������,����ֻ��һ������,Ȼ�������˾Ϳ���ʵ��
	// �����߼�,�����߼�������������ʵ��
	// ��ʱ������,������boss

private:
	CharacterManager();
	~CharacterManager();

public:
	static CharacterManager* instance();
	void on_input(const ExMessage& msg);
	void on_update(float delta);
	void on_render();
	Character* get_player();
	Character* get_enemy(const std::string& id = "");
	std::vector<Character*> get_all_enemy();

	void create_enemy(const std::string& id, std::shared_ptr<Character> enemy);
	void destroy_enemy(const std::string& id);
	void set_enable(bool flag) { is_enable = flag; }
};