#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "game.h"

#include <vector>

// 所有游戏对象都继承自Object
class Object
{
protected:
    Game& game_ = Game::getInstance();
    Object* parent_ = nullptr;
    std::vector<Object*> children_;

public:
    Object() = default;
    virtual ~Object();
    virtual void handleEvent(const SDL_Event& event);
    virtual void update(float delta_time);
    virtual void render();
};

#endif // _OBJECT_H_