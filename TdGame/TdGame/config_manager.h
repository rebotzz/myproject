#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include "singleton.h"
#include "tile.h"
#include "enemy_template.h"
#include "tower_template.h"
#include "player_template.h"
#include "wave.h"
#include "vector2.h"

#include "json/json.h"
#include "SDL.h"
#include <fstream>
#include <string>
#include <sstream>
#include <map>

class ConfigMgr : public Singleton<ConfigMgr>
{
	friend Singleton<ConfigMgr>;
public:
	typedef std::vector<SDL_Point> Route;	// ����·�ߣ����ӵ��±�

protected:
	ConfigMgr()
	{
		bool init = true;	
		init = parse_config() && init;		// debug:С��������·
		init = parse_level() && init;
		init = parse_map() && init;
		if (!init)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"��Ϸ����ʧ��", u8"�����ļ�����ʧ��", nullptr);
			exit(-1);
		}
	}
	~ConfigMgr() = default;

private:
	// ���������ļ�
	bool parse_config()
	{
		std::ifstream input("config/config.json");
		if (input.fail()) return false;
		std::stringstream str_stream;
		str_stream << input.rdbuf();
		input.close();

		Json::Value root;
		Json::Reader reader;
		if (!reader.parse(str_stream, root, false))
			return false;

		// ��Ϸ��������
		Json::Value& basic_root = root["basic"];
		if (basic_root.empty()) return false;
		window_width = basic_root["window_width"].asInt();
		window_height = basic_root["window_height"].asInt();
		window_title = basic_root["window_title"].asString();

		// �������
		Json::Value& player_root = root["player"];
		if (player_root.empty()) return false;
		player_template.speed = player_root["speed"].asFloat();
		player_template.normal_attack_interval = player_root["normal_attack_interval"].asFloat();
		player_template.normal_attack_damage = player_root["normal_attack_damage"].asFloat();
		player_template.skill_interval = player_root["skill_interval"].asFloat();
		player_template.skill_damage = player_root["skill_damage"].asFloat();

		// ����������
		Json::Value& tower_root = root["tower"];
		if (tower_root.empty()) return false;
		int idx = 0;

		{
			Json::Value& archer_root = tower_root["archer"];
			if (archer_root.empty()) return false;

			idx = 0;
			for (auto& x : archer_root["interval"])
				tower_archer_template.interval[idx++] = x.asFloat();
			idx = 0;
			for (auto& x : archer_root["damage"])
				tower_archer_template.damage[idx++] = x.asFloat();
			idx = 0;
			for (auto& x : archer_root["view_range"])
				tower_archer_template.view_range[idx++] = x.asFloat();
			idx = 0;
			for (auto& x : archer_root["cost"])
				tower_archer_template.cost[idx++] = x.asInt();
			idx = 0;
			for (auto& x : archer_root["upgrade_cost"])
				tower_archer_template.upgrade_cost[idx++] = x.asInt();
		}
		{
			Json::Value& axeman_root = tower_root["axeman"];
			if (axeman_root.empty()) return false;

			idx = 0;
			for (auto& x : axeman_root["interval"])
				tower_axeman_template.interval[idx++] = x.asFloat();
			idx = 0;
			for (auto& x : axeman_root["damage"])
				tower_axeman_template.damage[idx++] = x.asFloat();
			idx = 0;
			for (auto& x : axeman_root["view_range"])
				tower_axeman_template.view_range[idx++] = x.asFloat();
			idx = 0;
			for (auto& x : axeman_root["cost"])
				tower_axeman_template.cost[idx++] = x.asInt();
			idx = 0;
			for (auto& x : axeman_root["upgrade_cost"])
				tower_axeman_template.upgrade_cost[idx++] = x.asInt();
		}
		{
			Json::Value& gunner_root = tower_root["gunner"];
			if (gunner_root.empty()) return false;

			idx = 0;
			for (auto& x : gunner_root["interval"])
				tower_gunner_template.interval[idx++] = x.asFloat();
			idx = 0;
			for (auto& x : gunner_root["damage"])
				tower_gunner_template.damage[idx++] = x.asFloat();
			idx = 0;
			for (auto& x : gunner_root["view_range"])
				tower_gunner_template.view_range[idx++] = x.asFloat();
			idx = 0;
			for (auto& x : gunner_root["cost"])
				tower_gunner_template.cost[idx++] = x.asInt();
			idx = 0;
			for (auto& x : gunner_root["upgrade_cost"])
				tower_gunner_template.upgrade_cost[idx++] = x.asInt();
		}

		// ��������
		Json::Value& enemy_root = root["enemy"];
		if (enemy_root.empty()) return false;

		std::array<EnemyTemplate*, 5> enemy_template_list = { &enemy_slime_template , &enemy_king_slime_template ,
			&enemy_skeleton_template, &enemy_goblin_template , &enemy_goblin_priest_template };
		std::array<std::string, 5> enemy_object_list = { "slim", "king_slim", "skeleton", "goblin", "goblin_priest" };
		for (int i = 0; i < 5; ++i)
		{
			Json::Value& enemy_object = enemy_root[enemy_object_list[i]];
			if (enemy_object.empty()) return false;
			EnemyTemplate* enemy_template = enemy_template_list[i];
			enemy_template->hp = enemy_object["hp"].asInt();
			enemy_template->speed = enemy_object["speed"].asFloat();
			enemy_template->damage = enemy_object["damage"].asInt();
			enemy_template->reward_ratio = enemy_object["reward_ratio"].asFloat();
			enemy_template->recover_range = enemy_object["recover_range"].asFloat();
			enemy_template->recover_intensity = enemy_object["recover_intensity"].asFloat();
		}

		return true;
	}

	// �������˲���
	bool parse_level()
	{
		std::ifstream input("config/level.json");
		if (input.fail()) return false;
		std::stringstream str_stream;
		str_stream << input.rdbuf();
		input.close();

		Json::Value root;
		Json::Reader reader;
		if (!reader.parse(str_stream, root, false))
			return false;

		for (auto& wave_root : root)
		{
			if (wave_root.empty()) continue;

			wave_list.emplace_back();
			Wave& wave = wave_list.back();
			wave.rewards = wave_root["rewards"].asInt();
			wave.interval = wave_root["interval"].asFloat();
			Wave::SpawnList& spawn_list = wave.spawn_list;

			for (auto& spawn_root : wave_root["spawn_list"])
			{
				if (spawn_root.empty()) continue;

				spawn_list.emplace_back();
				Wave::SpawnEvent& spawn_event = spawn_list.back();
				spawn_event.interval = spawn_root["interval"].asFloat();
				spawn_event.point = spawn_root["point"].asInt();
				if (spawn_root["enemy"].asString() == "Slim")
					spawn_event.enemy = EnemyType::Slime;
				else if (spawn_root["enemy"].asString() == "KingSlim")
					spawn_event.enemy = EnemyType::KingSlime;
				else if (spawn_root["enemy"].asString() == "Skeleton")
					spawn_event.enemy = EnemyType::Skeleton;
				else if (spawn_root["enemy"].asString() == "Goblin")
					spawn_event.enemy = EnemyType::Goblin;
				else if (spawn_root["enemy"].asString() == "GoblinPriest")
					spawn_event.enemy = EnemyType::GoblinPriest;
			}
		}

		return true;
	}

	// ������ͼ
	bool parse_map()
	{
		std::ifstream input("config/map.csv");
		if (input.fail()) return false;
		std::stringstream str_stream;
		str_stream << input.rdbuf();
		input.close();

		std::string str_line;
		int row = 0, col = 0;
		std::map<int, SDL_Point> spawn_points_map;
		while (std::getline(str_stream, str_line))
		{
			col = 0;
			tile_map.emplace_back();
			auto& tile_map_row = tile_map.back();

			std::stringstream str_stream_line(str_line);
			std::string str_tile;
			while (std::getline(str_stream_line, str_tile, ','))
			{
				tile_map_row.emplace_back();
				Tile& tile = tile_map_row.back();

				std::stringstream str_stream_tile(str_tile);
				std::string str_tile_part;
				int idx = 0;
				// ����һ����Ƭ�Ĳ�ͬ�㼶
				while (std::getline(str_stream_tile, str_tile_part, '\\'))
				{
					int val = std::stoi(str_tile_part);
					if (idx == 0) tile.terrian = val;
					else if (idx == 1) tile.decoration = val;
					else if (idx == 2) tile.direction = static_cast<Tile::Direction>(val);
					else if (idx == 3) tile.speacial_flag = val;
					idx++;
				}

				if (tile.speacial_flag == 0)		// homeλ��
				{
					home.x = col;
					home.y = row;
				}
				else if (tile.speacial_flag > 0)	// ����ˢ�µ�
				{
					spawn_points_map[tile.speacial_flag] = { col, row };
				}
				col++;
			}
			row++;
		}

		std::vector<SDL_Point> spawn_points;
		for (auto& [index, point] : spawn_points_map)
			spawn_points.push_back(point);

		if (tile_map.empty() || tile_map[0].empty()) return false;
		// ��������·������ͼ
		create_route_list(spawn_points);	// debug:����ֱ���ڹ��캯�����ã���Ϊ��û������ɣ�����δ��ʼ��
		return true;
	}

	// ���ɵ���·��
	void create_route_list(const std::vector<SDL_Point>& spawn_points)
	{
		for (auto& spawn_point : spawn_points)
		{
			route_list.emplace_back();
			Route& route = route_list.back();

			int x = spawn_point.x, y = spawn_point.y;
			Tile::Direction direction = tile_map[y][x].direction;
			while (direction != Tile::Direction::None)
			{
				route.push_back({x, y});
				switch (direction)
				{
				case Tile::Direction::Up:		y--;	break;
				case Tile::Direction::Down:		y++;	break;
				case Tile::Direction::Left:		x--;	break;
				case Tile::Direction::Right:	x++;	break;
				}
				if (y < 0 || y > tile_map.size() || x < 0 || x > tile_map[0].size()) break;
				direction = tile_map[y][x].direction;
			}
			route.push_back({ x, y });
		}
	}

public:
	// todo �����ļ���û�е��ֶ�
	const int init_hp_max = 10;
	const int init_coin = 200;

public:
	// ��������
	int window_width = 1280, window_height = 720;
	std::string window_title = u8"TdGame";
	// �������
	PlayerTemplate player_template;
	// ����������
	TowerTemplate tower_archer_template;
	TowerTemplate tower_axeman_template;
	TowerTemplate tower_gunner_template;
	// ��������
	EnemyTemplate enemy_slime_template;
	EnemyTemplate enemy_king_slime_template;
	EnemyTemplate enemy_skeleton_template;
	EnemyTemplate enemy_goblin_template;
	EnemyTemplate enemy_goblin_priest_template;
	// ��ͼ��Ϣ
	TileMap tile_map;
	SDL_Point home;
	// ���˲�����Ϣ
	WaveList wave_list;
	std::vector<Route> route_list = { Route() };			// ˢ�µ��1��ʼ��0λ��ռ��
};


#endif // !_CONFIG_MANAGER_H_
