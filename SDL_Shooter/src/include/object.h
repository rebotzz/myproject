#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "SDL.h"
#include "vector2.h"
#include <functional>
#include <vector>

struct Object
{
    SDL_Texture* tex = nullptr;
    int width = 0;
    int height = 0;
    Vector2 pos;
    Vector2 direction;
    bool valid = true;
};

struct PlayerAttribute : Object
{
    int max_hp = 3;
    int current_hp = 3;
    double speed = 400;
    double shoot_cd = 0.1;
    double last_shoot_passed_time = 0;
    int damage = 1;
};

struct Bullet : Object
{
    double speed = 500;
    int damage = 1;
};

class AbstractPlayer;
struct Enemy : Object
{
    int current_hp = 1;
    double speed = 200;
    double shoot_cd = 0.8;
    double last_shoot_passed_time = 0;
    int damage = 1;
    int bullet_angle = 0;   // 角度制
    Bullet bullet_template;
    std::function<void(Enemy*, std::vector<Bullet*>&, AbstractPlayer*)> spawn_bullet;
};

enum class PropType
{
    Recover,
    Shield,
    Time
};

struct Prop : Object
{
    double speed = 150;
    int bounce_count = 3;   // 反弹次数
    PropType type = PropType::Recover;
};

struct ExplodeAnimation : Object
{
    int frame_idx = 0;
    int total_frame = 0;
    double frame_delta = 0.1;
    double pass_time = 0;
    double scale = 1.5; // 动画放大倍率
};

#endif //_OBJECT_H_
