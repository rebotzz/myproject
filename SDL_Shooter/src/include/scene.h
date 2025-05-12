#ifndef _SCENE_H_
#define _SCENE_H_

#include "SDL.h"

class GameMgr;

class Scene
{
public:
    Scene();
    virtual ~Scene() = default;
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void handleEvent(const SDL_Event& event) {};
    virtual void update(double delta_time) {};
    virtual void render() {};

protected:
    GameMgr& game_mgr;
};

#endif // _SCENE_H_