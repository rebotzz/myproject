#ifndef _ASSETS_STORE_
#define _ASSETS_STORE_

#include "../resID.h"	// 资源ID枚举,由createFilenameResIDMap()创建

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <unordered_map>
#include <string>
#include <filesystem>
#include <functional>


class AssetStore
{
private:
    // 字体包装器，实现不同字体大小
    class FontWrapper
    {
    public:
        FontWrapper(const std::string& path):font_path(path) {}
        ~FontWrapper();
        TTF_Font* getSize(int size);

    private:
        std::string font_path;
        std::unordered_map<int, TTF_Font*> size_pool;
    };
public:
	typedef std::unordered_map<ResID, SDL_Texture*> TexturePool;	// 图片纹理
	typedef std::unordered_map<ResID, Mix_Music*> MusicPool;		// 音乐
	typedef std::unordered_map<ResID, Mix_Chunk*> SoundPool;		// 音效
	typedef std::unordered_map<ResID, FontWrapper*> FontPool;		// 字体
	typedef std::unordered_map<ResID, std::string> TextPool;		// 文本
    typedef std::function<bool(const std::filesystem::path&, SDL_Renderer*)> Loader;   // 加载器

public:
	AssetStore();
	~AssetStore() = default;
	void load(SDL_Renderer* renderer, const std::string& resources_path);
    void unload();
	SDL_Texture* getTexture(ResID id);
	Mix_Music* getMusic(ResID id);
	Mix_Chunk* getSound(ResID id);
	TTF_Font* getFont(ResID id, int size);
	const std::string& getText(ResID id);

	// 工具函数：创建资源名字与资源ID映射(即创建两个头文件resID.h filename_to_resID.h)
	static void createFilenameResIDMap(const std::string& resources_dir);

private:
	TexturePool texture_pool;
	MusicPool music_pool;
	SoundPool sound_pool;
	FontPool font_pool;
	TextPool text_pool;
	std::unordered_map<std::string, ResID> nameID_map;      // 文件名字符串与ResID枚举映射
	std::unordered_map<ResID, std::string> IDname_map;      // ResID枚举与文件名字符串映射
    std::unordered_map<std::string, Loader> loader_pool;    // 不同文件格式的加载器
};

#endif // !_ASSETS_STORE_
