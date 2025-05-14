#ifndef _GAME_MGR_H_
#define _GAME_MGR_H_

#include "singleton.h"
#include "scene.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <string>
#include <map>
#include <functional>

class GameMgr : public Singleton<GameMgr>
{
    friend class Singleton<GameMgr>;
public:
    int run();

    void switchScene(Scene* scene);

    // set
    void setRankingList(const std::string& name, int score);
    void setScore(int val) { score = val; }

    // get
    int getWindowHeight() const { return window_h; }
    int getWindowWidth() const { return window_w; }
    SDL_Renderer* getRenderer() { return renderer; }
    const std::map<int, std::string, std::greater<int>>& getRankingList() const { return ranking_list; }
    int getScore() const { return score; }

    // 其他
    SDL_Point renderText(TTF_Font* font, const std::string& text, double pos_x_ratio, double pos_y_ratio, bool left_align = true);
    SDL_Point renderText(TTF_Font* font, const std::string& text, int pos_x, int pos_y);
    SDL_Point renderTextCenter(TTF_Font* font, const std::string& text, double pos_y_ratio);
private:
    GameMgr();
    ~GameMgr();
    void init();
    void clean();
    void renderBackground();
    void loadGame();
    void saveGame();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::string window_tittle = "<SDLShooter> -By rebotzz";
    bool is_quit = false;
    int window_h = 800;
    int window_w = 600;
    const double FPS = 60;
    Scene* current_scene = nullptr;
    std::map<int, std::string, std::greater<int>> ranking_list; // 排行榜
    int score = 0;  // 当前玩家得分
    bool is_full_screen = false;
};




#endif // _GAME_MGR_H_