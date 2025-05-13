#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.h"
#include "SDL.h"
#include <vector>
#include <functional>

class GameMgr;

class AbstractPlayer : protected PlayerAttribute
{
public:
    AbstractPlayer();
    virtual ~AbstractPlayer() {};

    virtual void handleEvent(const SDL_Event& event) {};
    virtual void update(double deltaTime) = 0;
    virtual void render() = 0;

    // get
    virtual const Vector2& get_pos() const { return pos; }
    virtual int get_current_hp() const { return current_hp; }
    virtual int get_max_hp() const { return max_hp; }
    virtual int get_height() const { return height; }
    virtual int get_width() const { return width; }
    virtual std::vector<Bullet*>& get_bullets() { return bullets; }
    bool get_wrapper_valid() { return wrapper_valid; }

    // set
    virtual void decrease_hp(int val) { current_hp -= val; if(current_hp < 0) current_hp = 0; }
    virtual void increase_hp(int val) { current_hp -= val; if(current_hp > max_hp) current_hp = max_hp; }
    virtual void set_spawn_bullet_callback(const std::function<void()>& callback) { spawn_bullet = callback; }
    AbstractPlayer* get_interal_player() { return player; }

    // 其他
    // void check_wrapper_valid()

protected:
    Bullet bullet_template;
    std::vector<Bullet*> bullets;
    GameMgr& game_mgr;
    AbstractPlayer* player = nullptr;
    std::function<void()> spawn_bullet; // 因为子弹生成在update中,不好修改，所以用仿函数
    bool wrapper_valid = true;
};

class BasePlayer : public AbstractPlayer
{
public:
    BasePlayer();
    ~BasePlayer();

    virtual void update(double deltaTime) override;
    virtual void render() override;
};

class ShieldPlayer : public AbstractPlayer
{
public:
    ShieldPlayer(AbstractPlayer* concrete_player);
    ~ShieldPlayer();

    virtual void update(double deltaTime);
    virtual void render() override;
    virtual void decrease_hp(int val);
    
    // 因为装饰者是"层层调用",所以这里需要手动写,不能再基类里搞定,因为不是"层层调用"
    virtual void set_spawn_bullet_callback(const std::function<void()>& callback) 
    { player->set_spawn_bullet_callback(callback); }
    // get
    virtual const Vector2& get_pos() const { return player->get_pos(); }
    virtual int get_current_hp() const { return player->get_current_hp(); }
    virtual int get_max_hp() const { return player->get_max_hp(); }
    virtual int get_height() const { return player->get_height(); }
    virtual int get_width() const { return player->get_width(); }
    virtual std::vector<Bullet*>& get_bullets() { return player->get_bullets(); }
    // set
    virtual void increase_hp(int val) { player->increase_hp(val); }

private:
    SDL_Texture* tex_origin_bullet = nullptr;
    SDL_Texture* tex_shield = nullptr;
    int shield_w = 0, shield_h = 0;
    double duration = 5.0;
};

// class TimePlayer : public AbstractPlayer
// {
// public:
//     TimePlayer(AbstractPlayer* concrete_player);
//     ~TimePlayer();

//     virtual void update(double deltaTime);
//     virtual void render();

//     // 因为装饰者是"层层调用",所以这里需要手动写,不能再基类里搞定,因为不是"层层调用"
//     virtual void set_spawn_bullet_callback(const std::function<void()>& callback) 
//     { player->set_spawn_bullet_callback(callback); }
//     // get
//     virtual const Vector2& get_pos() const { return player->get_pos(); }
//     virtual int get_current_hp() const { return player->get_current_hp(); }
//     virtual int get_max_hp() const { return player->get_max_hp(); }
//     virtual int get_height() const { return player->get_height(); }
//     virtual int get_width() const { return player->get_width(); }
//     virtual std::vector<Bullet*>& get_bullets() { return player->get_bullets(); }
//     // set
//     virtual void increase_hp(int val) { player->increase_hp(val); }
//     virtual void decrease_hp(int val) { player->decrease_hp(val); }

// private:
//     AbstractPlayer* player = nullptr;
//     double duration = 15.0;
// };

#endif // _PLAYER_H_