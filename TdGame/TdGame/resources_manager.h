#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include "singleton.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include <unordered_map>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>

// 枚举，减少资源查找时字符串hash过程
enum class ResID
{
	// 图片纹理
	Tex_BulletArrow,
	Tex_BulletAxe,
	Tex_BulletShell,
	Tex_Coin,
	Tex_EffectExplode,
	Tex_EffectFlashDown,
	Tex_EffectFlashLeft,
	Tex_EffectFlashRight,
	Tex_EffectFlashUp,
	Tex_EffectImpactDown,
	Tex_EffectImpactLeft,
	Tex_EffectImpactRight,
	Tex_EffectImpactUp,
	Tex_EnemyGoblin,
	Tex_EnemyGoblinPriest,
	Tex_EnemyGoblinPriestSketch,
	Tex_EnemyGoblinSketch,
	Tex_EnemyKingSlime,
	Tex_EnemyKingSlimeSketch,
	Tex_EnemySkeleton,
	Tex_EnemySkeletonSketch,
	Tex_EnemySlime,
	Tex_EnemySlimeSketch,
	Tex_Home,
	Tex_Player,
	Tex_Tileset,
	Tex_TowerArcher,
	Tex_TowerAxeman,
	Tex_TowerGunner,
	Tex_UiCoin,
	Tex_UiGameOverBar,
	Tex_UiHeart,
	Tex_UiHomeAvatar,
	Tex_UiLossText,
	Tex_UiPlaceHoveredLeft,
	Tex_UiPlaceHoveredRight,
	Tex_UiPlaceHoveredTop,
	Tex_UiPlaceIdle,
	Tex_UiPlayerAvatar,
	Tex_UiSelectCursor,
	Tex_UiUpgradeHoveredLeft,
	Tex_UiUpgradeHoveredRight,
	Tex_UiUpgradeHoveredTop,
	Tex_UiUpgradeIdle,
	Tex_UiWinText,

	// 背景音乐
	Mus_Bgm,

	// 音效
	Sound_ArrowFire_1,
	Sound_ArrowFire_2,
	Sound_ArrowHit_1,
	Sound_ArrowHit_2,
	Sound_ArrowHit_3,
	Sound_AxeFire,
	Sound_AxeHit_1,
	Sound_AxeHit_2,
	Sound_AxeHit_3,
	Sound_Coin,
	Sound_Flash,
	Sound_HomeHurt,
	Sound_Impact,
	Sound_Loss,
	Sound_PlaceTower,
	Sound_ShellFire,
	Sound_ShellHit,
	Sound_TowerLevelUp,
	Sound_Win,

	// 字体
	Font_Ipix,
};

class ResMgr : public Singleton<ResMgr>
{
	friend class Singleton<ResMgr>;		// 友元，方便基类访问子类构造
public:
	typedef std::unordered_map<ResID, SDL_Texture*> TexturePool;	// 图片纹理
	typedef std::unordered_map<ResID, Mix_Music*> MusicPool;		// 音乐
	typedef std::unordered_map<ResID, Mix_Chunk*> SoundPool;		// 音效
	typedef std::unordered_map<ResID, TTF_Font*> FontPool;			// 字体

public:
	bool load(SDL_Renderer* renderer, const std::string& resources_path)
	{
		//using namespace std::filesystem;
		if (!std::filesystem::exists(std::filesystem::path(resources_path))) return false;
		for (auto& entry : std::filesystem::recursive_directory_iterator(resources_path))
		{
			if (!entry.is_regular_file()) continue;
			auto& path = entry.path();
			if (path.extension().u8string() == ".png" || path.extension().u8string() == ".jpg")
			{
				texture_pool[name_map[path.stem().u8string()]] = IMG_LoadTexture(renderer, path.u8string().c_str());
			}
			else if (path.extension().u8string() == ".mp3" || path.extension().u8string() == ".wav")
			{
				if (path.stem().u8string().find("music") != std::string::npos)
					music_pool[name_map[path.stem().u8string()]] = Mix_LoadMUS(path.u8string().c_str());
				else 
					sound_pool[name_map[path.stem().u8string()]] = Mix_LoadWAV(path.u8string().c_str());
			}
			else if (path.extension().u8string() == ".ttf")
			{
				font_pool[name_map[path.stem().u8string()]] = TTF_OpenFont(path.u8string().c_str(), 25);
			}
		}

		return true;
	}

	SDL_Texture* find_texture(ResID id)
	{
		return texture_pool.count(id) ? texture_pool[id] : nullptr;
	}

	Mix_Music* find_music(ResID id)
	{
		return music_pool.count(id) ? music_pool[id] : nullptr;
	}

	Mix_Chunk* find_sound(ResID id)
	{
		return sound_pool.count(id) ? sound_pool[id] : nullptr;
	}

	TTF_Font* find_font(ResID id)
	{
		return font_pool.count(id) ? font_pool[id] : nullptr;
	}

	// 创建资源名字与资源ID映射
	static void create_NameResID_map(const std::string& resources_dir, const std::string& map_outfile)
	{
		std::ofstream output(map_outfile, std::ios::trunc | std::ios::out);
		if (!output.good()) return;

		std::vector<std::pair<std::string, std::string>> vs_tex;
		std::vector<std::pair<std::string, std::string>> vs_mus;
		std::vector<std::pair<std::string, std::string>> vs_sound;
		std::vector<std::pair<std::string, std::string>> vs_font;

		std::filesystem::path file_entry(resources_dir);
		if (!exists(file_entry)) return;

		for (auto& entry : std::filesystem::recursive_directory_iterator(resources_dir))
		{
			auto& path = entry.path();
			std::string id;
			if (path.extension() == ".png" || path.extension() == ".jpg")
			{
				id = "Tex_" + rename_ResID(path.stem().u8string());
				vs_tex.emplace_back(path.stem().u8string(), id);
			}
			else if (path.extension() == ".mp3" || path.extension() == ".wav")
			{
				std::string suffix = "Mus_";
				if (path.stem().u8string().find("sound") != std::string::npos)
				{
					suffix = "Sound_";
					int pos = path.stem().u8string().find("sound_");
					std::string tmp = path.stem().u8string().substr(pos == std::string::npos ? 0 : pos + strlen("sound_"));
					id = suffix + rename_ResID(tmp);
				}
				else
				{
					int pos = path.stem().u8string().find("music_");
					std::string tmp = path.stem().u8string().substr(pos == std::string::npos ? 0 : pos + strlen("music_"));
					id = suffix + rename_ResID(tmp);
				}

				if (suffix == "Mus_")
					vs_mus.emplace_back(path.stem().u8string(), id);
				else
					vs_sound.emplace_back(path.stem().u8string(), id);
			}
			if (path.extension() == ".ttf")
			{
				id = "Font_" + rename_ResID(path.stem().u8string());
				vs_font.emplace_back(path.stem().u8string(), id);
			}
		}

		output << "ResID: " << std::endl;
		for (auto& s : vs_tex)
			output << s.second << "," << std::endl;
		output << std::endl;
		for (auto& s : vs_mus)
			output << s.second << "," << std::endl;
		output << std::endl;
		for (auto& s : vs_sound)
			output << s.second << "," << std::endl;
		output << std::endl;
		for (auto& s : vs_font)
			output << s.second << "," << std::endl;
		output << std::endl;
		output << std::endl;

		output << "FileName-ResID:" << std::endl;
		for (auto& s : vs_tex)
			output << "{\"" << s.first << "\",\t ResID::" << s.second << "}," << std::endl;
		output << std::endl;
		for (auto& s : vs_mus)
			output << "{\"" << s.first << "\",\t  ResID::" << s.second << "}," << std::endl;
		output << std::endl;
		for (auto& s : vs_sound)
			output << "{\"" << s.first << "\",\t  ResID::" << s.second << "}," << std::endl;
		output << std::endl;
		for (auto& s : vs_font)
			output << "{\"" << s.first << "\",\t  ResID::" << s.second << "}," << std::endl;

		output.close();
	}

private:
	// 修改文件名为资源ID风格
	static std::string rename_ResID(const std::string& str)
	{
		if (str.empty()) return "";
		std::string s;
		s += _toupper(str[0]);
		for (int i = 1; i < str.size();)
		{
			if (str[i] != '_')
				s += str[i++];
			else if (i + 1 < str.size())
			{
				if (str[i + 1] >= '0' && str[i + 1] <= '9')
				{
					s += str.substr(i, std::string::npos);
					break;
				}
				else
				{
					s += _toupper(str[++i]);
					++i;
				}
			}
		}
		return s;
	};

protected:
	ResMgr()
	{
		name_map =
		{
			{"bullet_arrow",	 ResID::Tex_BulletArrow},
			{"bullet_axe",	 ResID::Tex_BulletAxe},
			{"bullet_shell",	 ResID::Tex_BulletShell},
			{"coin",	 ResID::Tex_Coin},
			{"effect_explode",	 ResID::Tex_EffectExplode},
			{"effect_flash_down",	 ResID::Tex_EffectFlashDown},
			{"effect_flash_left",	 ResID::Tex_EffectFlashLeft},
			{"effect_flash_right",	 ResID::Tex_EffectFlashRight},
			{"effect_flash_up",	 ResID::Tex_EffectFlashUp},
			{"effect_impact_down",	 ResID::Tex_EffectImpactDown},
			{"effect_impact_left",	 ResID::Tex_EffectImpactLeft},
			{"effect_impact_right",	 ResID::Tex_EffectImpactRight},
			{"effect_impact_up",	 ResID::Tex_EffectImpactUp},
			{"enemy_goblin",	 ResID::Tex_EnemyGoblin},
			{"enemy_goblin_priest",	 ResID::Tex_EnemyGoblinPriest},
			{"enemy_goblin_priest_sketch",	 ResID::Tex_EnemyGoblinPriestSketch},
			{"enemy_goblin_sketch",	 ResID::Tex_EnemyGoblinSketch},
			{"enemy_king_slime",	 ResID::Tex_EnemyKingSlime},
			{"enemy_king_slime_sketch",	 ResID::Tex_EnemyKingSlimeSketch},
			{"enemy_skeleton",	 ResID::Tex_EnemySkeleton},
			{"enemy_skeleton_sketch",	 ResID::Tex_EnemySkeletonSketch},
			{"enemy_slime",	 ResID::Tex_EnemySlime},
			{"enemy_slime_sketch",	 ResID::Tex_EnemySlimeSketch},
			{"home",	 ResID::Tex_Home},
			{"player",	 ResID::Tex_Player},
			{"tileset",	 ResID::Tex_Tileset},
			{"tower_archer",	 ResID::Tex_TowerArcher},
			{"tower_axeman",	 ResID::Tex_TowerAxeman},
			{"tower_gunner",	 ResID::Tex_TowerGunner},
			{"ui_coin",	 ResID::Tex_UiCoin},
			{"ui_game_over_bar",	 ResID::Tex_UiGameOverBar},
			{"ui_heart",	 ResID::Tex_UiHeart},
			{"ui_home_avatar",	 ResID::Tex_UiHomeAvatar},
			{"ui_loss_text",	 ResID::Tex_UiLossText},
			{"ui_place_hovered_left",	 ResID::Tex_UiPlaceHoveredLeft},
			{"ui_place_hovered_right",	 ResID::Tex_UiPlaceHoveredRight},
			{"ui_place_hovered_top",	 ResID::Tex_UiPlaceHoveredTop},
			{"ui_place_idle",	 ResID::Tex_UiPlaceIdle},
			{"ui_player_avatar",	 ResID::Tex_UiPlayerAvatar},
			{"ui_select_cursor",	 ResID::Tex_UiSelectCursor},
			{"ui_upgrade_hovered_left",	 ResID::Tex_UiUpgradeHoveredLeft},
			{"ui_upgrade_hovered_right",	 ResID::Tex_UiUpgradeHoveredRight},
			{"ui_upgrade_hovered_top",	 ResID::Tex_UiUpgradeHoveredTop},
			{"ui_upgrade_idle",	 ResID::Tex_UiUpgradeIdle},
			{"ui_win_text",	 ResID::Tex_UiWinText},

			{"music_bgm",	  ResID::Mus_Bgm},
			{"sound_arrow_fire_1",	  ResID::Sound_ArrowFire_1},
			{"sound_arrow_fire_2",	  ResID::Sound_ArrowFire_2},
			{"sound_arrow_hit_1",	  ResID::Sound_ArrowHit_1},
			{"sound_arrow_hit_2",	  ResID::Sound_ArrowHit_2},
			{"sound_arrow_hit_3",	  ResID::Sound_ArrowHit_3},
			{"sound_axe_hit_1",	  ResID::Sound_AxeHit_1},
			{"sound_axe_hit_2",	  ResID::Sound_AxeHit_2},
			{"sound_axe_hit_3",	  ResID::Sound_AxeHit_3},
			{"sound_coin",		ResID::Sound_Coin},
			{"sound_loss",		ResID::Sound_Loss},
			{"sound_place_tower",	  ResID::Sound_PlaceTower},
			{"sound_shell_hit",	  ResID::Sound_ShellHit},
			{"sound_tower_level_up",	  ResID::Sound_TowerLevelUp},

			{"sound_axe_fire",	  ResID::Sound_AxeFire},
			{"sound_flash",		  ResID::Sound_Flash},
			{"sound_home_hurt",	  ResID::Sound_HomeHurt},
			{"sound_impact",	  ResID::Sound_Impact},
			{"sound_shell_fire",  ResID::Sound_ShellFire},
			{"sound_win",		  ResID::Sound_Win},

			{"ipix",	  ResID::Font_Ipix},
		};
	}
	~ResMgr()
	{
		for (auto& [k, v] : texture_pool)
			if (v) SDL_DestroyTexture(v);
		for (auto& [k, v] : music_pool)
			if (v) Mix_FreeMusic(v);
		for (auto& [k, v] : sound_pool)
			if (v) Mix_FreeChunk(v);
		for (auto& [k, v] : font_pool)
			if (v) TTF_CloseFont(v);
	}

private:
	TexturePool texture_pool;
	MusicPool music_pool;
	SoundPool sound_pool;
	FontPool font_pool;

	std::unordered_map<std::string, ResID> name_map;
};

#endif // !_RESOURCES_MANAGER_H_
