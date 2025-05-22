#ifndef _GAME_H_
#define _GAME_H_

#include "singleton.h"
#include "assets_store.h"

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "glm/glm.hpp"
#include <string>
#include <random>

class Scene;

class Game : public Singleton<Game>
{
    friend class Singleton<Game>;
private:
    // 窗口相关 
    glm::vec2 screen_size_ = glm::vec2(1280.0f, 720.0f);
    SDL_Renderer* renderer_ = nullptr;
    SDL_Window* window_ = nullptr;
    bool is_running_ = true;
    TTF_TextEngine* text_engine_ = nullptr;     // 字体渲染引擎
    AssetStore asset_store_;                    // 素材资源
    glm::vec2 window_scale_ = glm::vec2(1.0f);  // 当前窗口缩放比例

    // 场景管理
    Scene* current_scene_ = nullptr;            
    Scene* scene_to_change_ = nullptr;
    // 帧率控制
    int FPS_ = 60;
    uint64_t last_frame_time_ = SDL_GetTicksNS();
    uint64_t delta_time_ = 0;
    uint64_t frame_interval_ = 0;
    // 其他
    std::mt19937 random_gen_;                   // 随机数生成器
    // 游戏存档
    int high_score_ = 0;    // 游戏数据暂时只有分数，所以只用一个int，之后可以改为string配合json

private:
    Game() = default;
    ~Game() = default;
    void handleEvent();
    void update(float delta_time);
    void render();
    void controlFrameDelta();
    void updateWindowScale();     // 更新因为屏幕缩放导致鼠标错位问题
    void saveGame();
    void loadGame();

public:
    void init(const std::string& tittle, int window_w, int window_h, int fps);
    void run();
    void clean();
    
    // 渲染函数
    void renderTTF_Text(TTF_Text* text, glm::vec2 pos, SDL_FColor color = {1.0f, 1.0f, 1.0f, 1.0f});
    void renderTexture(ResID res_id, SDL_FRect* src_rect, SDL_FRect* dst_rect) 
        { SDL_RenderTexture(renderer_, asset_store_.getTexture(res_id), src_rect, dst_rect); }
    void renderTexture(SDL_Texture* tex, SDL_FRect* src_rect, SDL_FRect* dst_rect) 
        { SDL_RenderTexture(renderer_, tex, src_rect, dst_rect); }
    void renderTexture(SDL_Texture* tex, SDL_FRect* src_rect, SDL_FRect* dst_rect, double angle, bool flip) 
        { SDL_RenderTextureRotated(renderer_, tex, src_rect, dst_rect, angle, nullptr, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE); }
    void renderGrid(glm::vec2 start, glm::vec2 end, float grid_w, float grid_h, SDL_FColor color = {1,1,1,1});
    void renderBoundary(glm::vec2 start, glm::vec2 end, int width, SDL_FColor color = {1,1,1,1});
    void renderFillRect(glm::vec2 position, glm::vec2 size, SDL_FColor color = {1,1,1,1});

    // 工具函数
    void changeScene(Scene* scene);        // 不安全，可能会出现delete this，类调用的函数delete自生，如果接下来还有逻辑，可能出错
    void safeChangeScene(Scene* scene) { scene_to_change_ = scene; }
    TTF_Text* createTTF_Text(const std::string& text, TTF_Font* font, size_t text_length = 0)  //debug:这里length是文本长度，不是一行长度
        { return TTF_CreateText(text_engine_, font, text.c_str(), text_length); }
    float getRandom(float begin, float end) { return std::uniform_real_distribution<float>(begin, end)(random_gen_); }
    int getRandom(int begin, int end) { return std::uniform_int_distribution<int>(begin, end)(random_gen_); }
    glm::vec2 getRandomVec2(const glm::vec2& begin, const glm::vec2& end);
    void pauseMusic() { Mix_PausedMusic(); }
    void pauseSound() { Mix_Pause(-1); }
    void resumeMusic() { Mix_ResumeMusic(); }
    void resumeSound() { Mix_Resume(-1); }
    void playMusic(ResID mus_id) { Mix_PlayMusic(asset_store_.getMusic(mus_id), -1); }
    void playSound(ResID sound_id) { Mix_PlayChannel(-1, asset_store_.getSound(sound_id), 0); }
    void quit() { is_running_ = false; }
    void updateGameData(int score) { high_score_ = std::max(high_score_, score); }
    int getGameData() { return high_score_; }
    SDL_MouseButtonFlags getMouseState(glm::vec2& mouse_position);

    // getters and setters
    glm::vec2 getScreenSize() const { return screen_size_; }
    AssetStore& getAssetStore() { return asset_store_; }
    void setRenderColor(SDL_FColor color = {1.0f, 1.0f, 1.0f, 1.0f}) { SDL_SetRenderDrawColorFloat(renderer_, color.r, color.g, color.b, color.a); }
};

#endif // _GAME_H_
