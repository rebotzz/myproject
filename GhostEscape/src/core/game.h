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
    glm::ivec2 screen_size_ = glm::ivec2(1280, 720);
    int FPS_ = 60;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Window* window_ = nullptr;
    bool is_running_ = true;
    glm::vec2 mouse_pos_;
    TTF_TextEngine* text_engine_ = nullptr;     // 字体渲染引擎
    AssetStore asset_store_;                    // 素材资源
    Scene* current_scene_ = nullptr;            
    std::mt19937 random_gen_;                   // 随机数生成器

public:
    void init(const std::string& tittle, int window_w, int window_h, int fps);
    void run();
    void clean();
    
    // 渲染函数
    void renderText(TTF_Text* text, glm::vec2 pos) { TTF_DrawRendererText(text, pos.x, pos.y); }
    void renderTexture(ResID res_id, SDL_FRect* src_rect, SDL_FRect* dst_rect) 
        { SDL_RenderTexture(renderer_, asset_store_.getTexture(res_id), src_rect, dst_rect); }
    void renderTexture(SDL_Texture* tex, SDL_FRect* src_rect, SDL_FRect* dst_rect) 
        { SDL_RenderTexture(renderer_, tex, src_rect, dst_rect); }
    void renderTexture(SDL_Texture* tex, SDL_FRect* src_rect, SDL_FRect* dst_rect, double angle, bool flip) 
        { SDL_RenderTextureRotated(renderer_, tex, src_rect, dst_rect, angle, nullptr, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE); }
    void drawGrid(glm::vec2 start, glm::vec2 end, float grid_w, float grid_h, SDL_FColor color = {1,1,1,1});
    void renderBoundary(glm::vec2 start, glm::vec2 end, int width, SDL_FColor color = {1,1,1,1});
    void renderFillRect(glm::vec2 position, glm::vec2 size, SDL_FColor color = {1,1,1,1});

    // 工具函数
    void changeScene(Scene* scene);
    TTF_Text* createText(const std::string& text, TTF_Font* font, size_t text_length = 0) { return TTF_CreateText(text_engine_, font, text.c_str(), text_length); }
    float getRandom(float begin, float end) { return std::uniform_real_distribution<float>(begin, end)(random_gen_); }
    int getRandom(int begin, int end) { return std::uniform_int_distribution<int>(begin, end)(random_gen_); }
    glm::vec2 getRandomVec2(const glm::vec2& begin, const glm::vec2& end);

    // getters and setters
    glm::vec2 getScreenSize() const { return screen_size_; }
    SDL_Renderer* getRenderer() { return renderer_; }
    AssetStore& getAssetStore() { return asset_store_; }

private:
    Game() = default;
    ~Game() = default;
    void handleEvent();
    void update(float delta_time);
    void render();
};

#endif // _GAME_H_
