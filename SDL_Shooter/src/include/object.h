#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "SDL.h"
#include "vector2.h"


struct Object
{
    SDL_Texture* tex = nullptr;
    int width = 0;
    int height = 0;
    Vector2 pos;
};

struct Player : Object
{
    int max_hp = 3;
    int current_hp = 3;
    double speed = 500;
    double shoot_cd = 0.1;
    double last_shoot_time = 0;
};

struct Enemy : Object
{
    int current_hp = 1;
    double speed = 200;
    double shoot_cd = 0.3;
    double last_shoot_time = 0;
};

struct Bullet : Object
{
    double speed = 500;
    bool valid = true;
};

struct Prop : Object
{
    double speed = 500;
    bool valid = true;
};

#endif //_OBJECT_H_
