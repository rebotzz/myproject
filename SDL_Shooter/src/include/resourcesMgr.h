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
#include <functional>

// 枚举，减少资源查找时字符串hash过程
enum class ResID
{
	// 图片纹理
    Tex_Explosion,
    Tex_Bg,
    Tex_BonusLife,
    Tex_BonusShield,
    Tex_BonusTime,
    Tex_Bullet,
    Tex_Bullet1,
    Tex_Bullet2,
    Tex_Fire,
    Tex_HealthUIBlack,
    Tex_Insect1,
    Tex_Insect2,
    Tex_LargeA,
    Tex_LargeB,
    Tex_Laser1,
    Tex_Laser2,
    Tex_Laser3,
    Tex_MediumA,
    Tex_MediumB,
    Tex_Plasm,
    Tex_Rocket,
    Tex_Shield,
    Tex_SmallA,
    Tex_SmallB,
    Tex_SpaceShip,
    Tex_StarsA,
    Tex_StarsB,
    Tex_Support,
	// 背景音乐
    Mus_BattleInSpaceIntro,
    Mus_RacingThroughAsteroidsLoop,
	// 音效
    Sound_Eff11,
    Sound_Eff5,
    Sound_Explosion1,
    Sound_Explosion3,
    Sound_LaserShoot4,
    Sound_XsLaser,
    Sound_PlayerHurt,
	// 字体
    Font_VonwaonBitmap12px,
    Font_VonwaonBitmap16px,
};

class ResMgr : public Singleton<ResMgr>
{
	friend class Singleton<ResMgr>;		// 友元，方便基类访问子类构造
private:
    // 字体包装器，实现不同字体大小
    class FontWrapper
    {
    public:
        FontWrapper(const std::string& path):font_path(path) {}
        ~FontWrapper();
        TTF_Font* get_font(int size);

    private:
        std::string font_path;
        std::unordered_map<int, TTF_Font*> font_pool;
    };
public:
	typedef std::unordered_map<ResID, SDL_Texture*> TexturePool;	// 图片纹理
	typedef std::unordered_map<ResID, Mix_Music*> MusicPool;		// 音乐
	typedef std::unordered_map<ResID, Mix_Chunk*> SoundPool;		// 音效
	typedef std::unordered_map<ResID, FontWrapper*> FontPool;		// 字体
    typedef std::function<bool(const std::filesystem::path&, SDL_Renderer*)> Loader;   // 加载器

public:
	bool load(SDL_Renderer* renderer, const std::string& resources_path);
    void unload();

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

	TTF_Font* find_font(ResID id, int size)
	{
		return font_pool.count(id) ? font_pool[id]->get_font(size) : nullptr;
	}

	// 创建资源名字与资源ID映射
	static void create_NameResID_map(const std::string& resources_dir, const std::string& map_outfile);

protected:
	ResMgr();
	~ResMgr() = default;

private:
	TexturePool texture_pool;
	MusicPool music_pool;
	SoundPool sound_pool;
	FontPool font_pool;
	std::unordered_map<std::string, ResID> nameId_map;      // 文件名字符串与ResID枚举映射
    std::unordered_map<std::string, Loader> loader_pool;    // 不同文件格式的加载器
};

#endif // !_RESOURCES_MANAGER_H_
