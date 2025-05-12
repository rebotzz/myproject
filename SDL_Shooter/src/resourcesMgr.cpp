#include "resourcesMgr.h"
#include <fstream>
#include <vector>

ResMgr::FontWrapper::~FontWrapper()
{
    for(auto& [size, font] : font_pool)
        if(font) TTF_CloseFont(font);
    font_pool.clear();
}

TTF_Font* ResMgr::FontWrapper::get_font(int size)
{
    if(!font_pool.count(size))
    {
        TTF_Font* font = TTF_OpenFont(font_path.c_str(), size);
        if(!font) 
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "open font [%s] failed: %s", font_path.c_str(), SDL_GetError());
            return nullptr;
        }
        font_pool[size] = font;
    }
    return font_pool[size];
}

ResMgr::ResMgr()
{
    // 加载器初始化
    loader_pool[".png"] = [this](const std::filesystem::path& path, SDL_Renderer* renderer)->bool
    {
        SDL_Texture* tex = IMG_LoadTexture(renderer, path.u8string().c_str());
        if(!tex) 
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load texture [%s] failed: %s", path.u8string().c_str(), SDL_GetError());
            return false;
        }
        texture_pool[nameId_map[path.stem().u8string()]] = tex;
        return true;
    };

    loader_pool[".ogg"] = loader_pool[".wav"] = [this](const std::filesystem::path& path, SDL_Renderer* renderer)->bool
    {
        // Mix_Music一边播放一边加载,适合bgm
        if (path.stem().u8string().find("music") != std::string::npos)
        {
            Mix_Music* mus = Mix_LoadMUS(path.u8string().c_str());
            if(!mus)
            {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load Mix_Music [%s] failed: %s", path.u8string().c_str(), SDL_GetError());
                return false;
            }
            music_pool[nameId_map[path.stem().u8string()]] = mus;
        }
        // Mix_Chunk直接加载，适合短的音效
        else 
        {
            Mix_Chunk* chunk = Mix_LoadWAV(path.u8string().c_str());
            if(!chunk)
            {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load Mix_Chunk [%s] failed: %s", path.u8string().c_str(), SDL_GetError());
                return false;
            }
            sound_pool[nameId_map[path.stem().u8string()]] = chunk;
        }
        return true;
    };

    loader_pool[".ttf"] = [this](const std::filesystem::path& path, SDL_Renderer* renderer)->bool
    {
        font_pool[nameId_map[path.stem().u8string()]] = new FontWrapper(path.u8string());
        return true;
    };

    // 初始化str文件名与ResID枚举映射
    nameId_map =
    {
        {"explosion",	 ResID::Tex_Explosion},
        {"bg",	 ResID::Tex_Bg},
        {"bonus_life",	 ResID::Tex_BonusLife},
        {"bonus_shield",	 ResID::Tex_BonusShield},
        {"bonus_time",	 ResID::Tex_BonusTime},
        {"bullet",	 ResID::Tex_Bullet},
        {"bullet_1",	 ResID::Tex_Bullet1},
        {"bullet_2",	 ResID::Tex_Bullet2},
        {"fire",	 ResID::Tex_Fire},
        {"Health UI Black",	 ResID::Tex_HealthUIBlack},
        {"insect_1",	 ResID::Tex_Insect1},
        {"insect_2",	 ResID::Tex_Insect2},
        {"large_A",	 ResID::Tex_LargeA},
        {"large_B",	 ResID::Tex_LargeB},
        {"laser_1",	 ResID::Tex_Laser1},
        {"laser_2",	 ResID::Tex_Laser2},
        {"laser_3",	 ResID::Tex_Laser3},
        {"medium_A",	 ResID::Tex_MediumA},
        {"medium_B",	 ResID::Tex_MediumB},
        {"plasm",	 ResID::Tex_Plasm},
        {"rocket",	 ResID::Tex_Rocket},
        {"shield",	 ResID::Tex_Shield},
        {"small_A",	 ResID::Tex_SmallA},
        {"small_B",	 ResID::Tex_SmallB},
        {"SpaceShip",	 ResID::Tex_SpaceShip},
        {"Stars_A",	 ResID::Tex_StarsA},
        {"Stars_B",	 ResID::Tex_StarsB},
        {"support",	 ResID::Tex_Support},

        {"Battle_in_Space_Intro",	  ResID::Mus_BattleInSpaceIntro},
        {"Racing_Through_Asteroids_Loop",	  ResID::Mus_RacingThroughAsteroidsLoop},

        {"eff11",	  ResID::Sound_Eff11},
        {"eff5",	  ResID::Sound_Eff5},
        {"explosion1",	  ResID::Sound_Explosion1},
        {"explosion3",	  ResID::Sound_Explosion3},
        {"laser_shoot4",	  ResID::Sound_LaserShoot4},
        {"xs_laser",	  ResID::Sound_XsLaser},

        {"VonwaonBitmap_12px",	  ResID::Font_VonwaonBitmap12px},
        {"VonwaonBitmap_16px",	  ResID::Font_VonwaonBitmap16px},
    };
}

bool ResMgr::load(SDL_Renderer* renderer, const std::string& resources_path)
{
    if (!std::filesystem::exists(std::filesystem::path(resources_path))) return false;
    for (auto& entry : std::filesystem::recursive_directory_iterator(resources_path))
    {
        if (!entry.is_regular_file()) continue;
        auto& path = entry.path();
        if(loader_pool.count(path.extension().u8string()))
            loader_pool[path.extension().u8string()](path, renderer);
    }

    return true;
}

void ResMgr::unload()
{
    for (auto& [k, v] : texture_pool)
        if (v) SDL_DestroyTexture(v);
    for (auto& [k, v] : music_pool)
        if (v) Mix_FreeMusic(v);
    for (auto& [k, v] : sound_pool)
        if (v) Mix_FreeChunk(v);
    for (auto& [k, v] : font_pool)
        if (v) delete v;

    texture_pool.clear();
    music_pool.clear();
    sound_pool.clear();
    font_pool.clear();
}

// 修改文件名为资源ID风格
static std::string rename_ResID(const std::string& str)
{
    if (str.empty()) return "";
    // 将"npc_idle_1" 改为 "NpcIdle1"; 注意:命名只能最好只有字母/数字/下划线
    std::string s;
    bool upper_alpha = true;
    for (int i = 0; i < str.size(); i++)
    {
        // 字母/数字/下划线之外,跳过
        if(!isalnum(str[i]) && str[i] != '_') continue;
        // 下划线,下一个字母大写
        else if(str[i] == '_') upper_alpha = true;
        else
        {
            s += str[i];
            if(islower(str[i]) && upper_alpha) s.back() = _toupper(str[i]);
            upper_alpha = false;
        }
    }
    return s;
};

// 创建资源名字与资源ID映射
void ResMgr::create_NameResID_map(const std::string& resources_dir, const std::string& map_outfile)
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
        else if (path.extension() == ".mp3" || path.extension() == ".wav" || path.extension() == ".ogg")
        {
            std::string suffix = "Mus_";
            // 在sound文件夹中存放音效
            if (path.relative_path().u8string().find("sound") != std::string::npos)
            {
                suffix = "Sound_";
            }
            id = suffix + rename_ResID(path.stem().u8string());

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


