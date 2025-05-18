#include "assets_store.h"
#include "../filename_to_resID.h"   // 文件名到资源id映射,由createFilenameResIDMap()创建

#include <fstream>
#include <sstream>
#include <vector>

AssetStore::FontWrapper::~FontWrapper()
{
    for(auto& [size, font] : size_pool)
        if(font) TTF_CloseFont(font);
    size_pool.clear();
}

TTF_Font* AssetStore::FontWrapper::getSize(int size)
{
    if(!size_pool.count(size))
    {
        TTF_Font* font = TTF_OpenFont(font_path.c_str(), static_cast<float>(size));
        if(!font) 
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "open font [%s] failed: %s", font_path.c_str(), SDL_GetError());
            return nullptr;
        }
        size_pool[size] = font;
    }
    return size_pool[size];
}

AssetStore::AssetStore()
{
    // 加载器初始化
    loader_pool[".png"] = loader_pool[".jpg"] = 
    [this](const std::filesystem::path& path, SDL_Renderer* renderer)->bool
    {
        if(!nameID_map.count(path.stem().u8string())) return false;
        SDL_Texture* tex = IMG_LoadTexture(renderer, path.u8string().c_str());
        if(!tex) 
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load texture [%s] failed: %s", path.u8string().c_str(), SDL_GetError());
            return false;
        }
        texture_pool[nameID_map[path.stem().u8string()]] = tex;
        return true;
    };

    loader_pool[".mp3"] = loader_pool[".ogg"] = loader_pool[".wav"] = 
    [this](const std::filesystem::path& path, SDL_Renderer* )->bool
    {
        if(!nameID_map.count(path.stem().u8string())) return false;
        // Mix_Music一边播放一边加载,适合bgm
        if (path.u8string().find("music") != std::string::npos)
        {
            Mix_Music* mus = Mix_LoadMUS(path.u8string().c_str());
            if(!mus)
            {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load Mix_Music [%s] failed: %s", path.u8string().c_str(), SDL_GetError());
                return false;
            }
            music_pool[nameID_map[path.stem().u8string()]] = mus;
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
            sound_pool[nameID_map[path.stem().u8string()]] = chunk;
        }
        return true;
    };

    loader_pool[".ttf"] = [this](const std::filesystem::path& path, SDL_Renderer* )->bool
    {
        if(!nameID_map.count(path.stem().u8string())) return false;
        font_pool[nameID_map[path.stem().u8string()]] = new FontWrapper(path.u8string());
        return true;
    };

    loader_pool[".txt"] = [this](const std::filesystem::path& path, SDL_Renderer* )->bool
    {
        if(!nameID_map.count(path.stem().u8string())) return false;
        std::ifstream input(path.u8string());
        if(!input.good()) return false;
        std::stringstream str_stream;
        str_stream << input.rdbuf();
        input.close();
        std::string text, tmp;
        while(std::getline(str_stream, tmp))
        {
            text += tmp;
        }
        text_pool.insert({nameID_map[path.stem().u8string()], text});
        return true;
    };

    // 初始化str文件名与ResID枚举映射
    nameID_map = filename_resID;
    for(auto& [name, id] : nameID_map)
    {
        IDname_map.insert({id, name});
    }
}

void AssetStore::load(SDL_Renderer* renderer, const std::string& resources_path)
{
    if (!std::filesystem::exists(std::filesystem::path(resources_path))) 
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "asset path [%s] not exists!", resources_path.c_str());
        exit(-1);
    }
    for (auto& entry : std::filesystem::recursive_directory_iterator(resources_path))
    {
        if (!entry.is_regular_file()) continue;
        auto& path = entry.path();
        if(loader_pool.count(path.extension().u8string()))
        {
            if(!loader_pool[path.extension().u8string()](path, renderer))
            {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load resources [%s] failed: %s", 
                    path.u8string().c_str(), SDL_GetError());
            }
        }
        else 
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "undefine resources type [%s]", 
                path.u8string().c_str());
        }
    }
}

void AssetStore::unload()
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
    text_pool.clear();
}

SDL_Texture *AssetStore::getTexture(ResID id)
{
    if(texture_pool.find(id) != texture_pool.end()) return texture_pool[id];
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "not find resources: %s", IDname_map[id].c_str());
    return nullptr;
}

Mix_Music *AssetStore::getMusic(ResID id)
{
    if(music_pool.find(id) != music_pool.end()) return music_pool[id];
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "not find resources: %s", IDname_map[id].c_str());
    return nullptr;
}

Mix_Chunk* AssetStore::getSound(ResID id)
{
    if(sound_pool.find(id) != sound_pool.end()) return sound_pool[id];
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "not find resources: %s", IDname_map[id].c_str());
    return nullptr;
}

TTF_Font* AssetStore::getFont(ResID id, int size)
{
    auto font = font_pool[id]->getSize(size);
    if(font) return font;
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "not find resources: %s", IDname_map[id].c_str());
    return nullptr;
}

const std::string& AssetStore::getText(ResID id)
{
    if(text_pool.find(id) != text_pool.end()) return text_pool[id];
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "not find resources: %s", IDname_map[id].c_str());
    return "";
}


// 修改文件名为资源ID风格
static std::string renameResID(const std::string& str)
{
    if (str.empty()) return "";
    // 将"npc_idle_1" 改为 "NpcIdle1"; 注意:命名只能最好只有字母/数字/下划线
    std::string s;
    bool upper_alpha = true;
    for (size_t i = 0; i < str.size(); i++)
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

void AssetStore::createFilenameResIDMap(const std::string& resources_dir)
{
    std::vector<std::pair<std::string, std::string>> vs_tex;
    std::vector<std::pair<std::string, std::string>> vs_mus;
    std::vector<std::pair<std::string, std::string>> vs_sound;
    std::vector<std::pair<std::string, std::string>> vs_font;
    std::vector<std::pair<std::string, std::string>> vs_text;

    std::filesystem::path file_entry(resources_dir);
    if (!exists(file_entry)) return;

    for (auto& entry : std::filesystem::recursive_directory_iterator(resources_dir))
    {
        auto& path = entry.path();
        std::string id;
        if (path.extension() == ".png" || path.extension() == ".jpg")
        {
            id = "Tex_" + renameResID(path.stem().u8string());
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
            id = suffix + renameResID(path.stem().u8string());

            if (suffix == "Mus_")
                vs_mus.emplace_back(path.stem().u8string(), id);
            else
                vs_sound.emplace_back(path.stem().u8string(), id);
        }
        else if (path.extension() == ".ttf")
        {
            id = "Font_" + renameResID(path.stem().u8string());
            vs_font.emplace_back(path.stem().u8string(), id);
        }
        else if(path.extension() == ".txt")
        {
            id = "Text_" + renameResID(path.stem().u8string());
            vs_text.emplace_back(path.stem().u8string(), id);
        }
    }

    // resID.h头文件
    std::ofstream output("resID.h", std::ios::trunc | std::ios::out);
    if (!output.good()) return;
    output << "#ifndef _RES_ID_H_\n";
    output << "#define _RES_ID_H_\n\n";
    output << "enum class ResID\n{" << std::endl;
    output << "\t//图片纹理" << std::endl;
    for (auto& s : vs_tex)
        output << "\t" << s.second << "," << std::endl;
    output << "\t//BGM音频" << std::endl;
    for (auto& s : vs_mus)
        output << "\t" << s.second << "," << std::endl;
    output << "\t//音效" << std::endl;
    for (auto& s : vs_sound)
        output << "\t" << s.second << "," << std::endl;
    output << "\t//字体" << std::endl;
    for (auto& s : vs_font)
        output << "\t" << s.second << "," << std::endl;
    output << "\t//文本" << std::endl;
    for (auto& s : vs_text)
        output << "\t" << s.second << "," << std::endl;
    output << "};\n"<< std::endl;
    output << std::endl;
    output << "#endif // _RES_ID_H_\n\n";
    output.close();

    // filename_to_resID.h头文件
    output.open("filename_to_resID.h", std::ios::trunc | std::ios::out);
    if (!output.good()) return;
    output << "#ifndef _FILENAME_TO_RESID_H_\n";
    output << "#define _FILENAME_TO_RESID_H_\n\n";
    output << "#include \"resID.h\"\n";
    output << "#include <unordered_map>\n";
    output << "#include <string>\n\n";
    output << "std::unordered_map<std::string, ResID> filename_resID = \n{" << std::endl;
    for (auto& s : vs_tex)
        output << "\t{\"" << s.first << "\",\t ResID::" << s.second << "}," << std::endl;
    output << std::endl;
    for (auto& s : vs_mus)
        output << "\t{\"" << s.first << "\",\t  ResID::" << s.second << "}," << std::endl;
    output << std::endl;
    for (auto& s : vs_sound)
        output << "\t{\"" << s.first << "\",\t  ResID::" << s.second << "}," << std::endl;
    output << std::endl;
    for (auto& s : vs_font)
        output << "\t{\"" << s.first << "\",\t  ResID::" << s.second << "}," << std::endl;
    for (auto& s : vs_text)
        output << "\t{\"" << s.first << "\",\t  ResID::" << s.second << "}," << std::endl;
    output << "};\n"; 
    output << "#endif // _FILENAME_TO_RESID_H_\n\n";

    output.close();
}


