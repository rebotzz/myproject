#ifndef _GAME_MGR_H_
#define _GAME_MGR_H_

#include "singleton.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <string>

class GameMgr : public Singleton<GameMgr>
{
    friend class Singleton<GameMgr>;
public:
    int run();


    // set

    // get

private:
    GameMgr();
    ~GameMgr();
    void init();
    void clean();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::string window_tittle = "SDLShooter";
    bool is_quit = false;
    int window_h = 800;
    int window_w = 600;

};




#endif // _GAME_MGR_H_