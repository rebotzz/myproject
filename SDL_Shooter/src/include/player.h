#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.h"
#include "SDL.h"
#include <vector>
#include <functional>

class GameMgr;
class MiddlePlayer;
class ShieldPlayer;
class TimePlayer;

class AbstractPlayer : protected PlayerAttribute
{
    friend class MiddlePlayer;
    friend class ShieldPlayer;
    friend class TimePlayer;
public:
    typedef std::function<void(std::vector<Bullet*>&, const Vector2& player_pos, 
        int player_w, int player_h, const Bullet& bullet_template)> SpawnBulletCallback;
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
    bool get_wrapper_valid() { return wrapper_valid; }     // 本层装饰者是否有效

    // set
    virtual void decrease_hp(int val) { current_hp -= val; if(current_hp < 0) current_hp = 0; }
    virtual void increase_hp(int val) { current_hp += val; if(current_hp > max_hp) current_hp = max_hp; }
    AbstractPlayer* get_interal_player() { return player; } // 返回本层装饰者修饰的对象

protected:
    // 其他装饰者修改接口
    virtual SDL_Texture* get_texture() { return tex; }
    virtual double get_speed() { return speed; }
    virtual double get_shoot_cd() { return shoot_cd; }
    virtual int get_damage() { return damage; }
    virtual const SpawnBulletCallback& get_spawn_bullet_callback() { return spawn_bullet; }
    virtual const Bullet& get_bullet_template() { return bullet_template; }

    virtual void set_spawn_bullet_callback(const SpawnBulletCallback& callback) { spawn_bullet = callback; }
    virtual void set_texture(SDL_Texture* texture) { tex = texture; }
    virtual void set_speed(double val) { speed = val; }
    virtual void set_shoot_cd(double val) { shoot_cd = val; }
    virtual void set_max_hp(double val) { max_hp = val; }
    virtual void set_damage(double val) { damage = val; }
    virtual void set_height(int val) { height = val; }
    virtual void set_width(int val) { width = val; }
    virtual void set_bullet_template(const Bullet& val) { bullet_template = val; }

protected:
    Bullet bullet_template;
    std::vector<Bullet*> bullets;
    GameMgr& game_mgr;
    AbstractPlayer* player = nullptr;
    SpawnBulletCallback spawn_bullet; // 因为子弹生成在update中,不好修改，所以用仿函数,Debug:不能捕获外层包装参数，会失效
    bool wrapper_valid = true;
};

// 装饰者最内层,永不失效,且内部没有指向的装饰者
class BasePlayer : public AbstractPlayer
{
public:
    BasePlayer();
    ~BasePlayer();

    virtual void update(double deltaTime) override;
    virtual void render() override;
};

// 只是为了避免后续每个继承类都重写虚函数,无其他作用;
// 其实也可以放在基类AbstractPlayer,不过需要判断player是否为空
class MiddlePlayer : public AbstractPlayer
{
public:
    // 因为装饰者是"层层调用",所以这里需要手动写,不能再基类里搞定,因为不是"层层调用",每层都有一个player
    // get
    virtual const Vector2& get_pos() const { return player->get_pos(); }
    virtual int get_current_hp() const { return player->get_current_hp(); }
    virtual int get_max_hp() const { return player->get_max_hp(); }
    virtual int get_height() const { return player->get_height(); }
    virtual int get_width() const { return player->get_width(); }
    virtual std::vector<Bullet*>& get_bullets() { return player->get_bullets(); }
    // set
    virtual void increase_hp(int val) { player->increase_hp(val); }

protected:
    virtual SDL_Texture* get_texture() { return player->get_texture(); }
    virtual double get_speed() { return player->get_speed(); }
    virtual double get_shoot_cd() { return player->get_shoot_cd(); }
    virtual int get_damage() { return player->get_damage(); }
    virtual const SpawnBulletCallback& get_spawn_bullet_callback() { return player->get_spawn_bullet_callback(); }
    virtual const Bullet& get_bullet_template() { return player->get_bullet_template(); }

    virtual void set_spawn_bullet_callback(const SpawnBulletCallback& callback) 
    { player->set_spawn_bullet_callback(callback); }
    virtual void set_texture(SDL_Texture* texture) { player->set_texture(texture); }
    virtual void set_speed(double val) { player->set_speed(val); }
    virtual void set_shoot_cd(double val) { player->set_shoot_cd(val); }
    virtual void set_max_hp(double val) { player->set_max_hp(val); }
    virtual void set_damage(double val) { player->set_damage(val); }
    virtual void set_height(int val) { player->set_height(val); }
    virtual void set_width(int val) { player->set_width(val); }
    virtual void set_bullet_template(const Bullet& val) { player->set_bullet_template(val); }
};

// 临时护盾(叠加时仅重置护盾持续时间)
class ShieldPlayer : public MiddlePlayer
{
public:
    ShieldPlayer(AbstractPlayer* concrete_player);
    ~ShieldPlayer();

    virtual void update(double deltaTime);
    virtual void render() override;
    virtual void decrease_hp(int val);
    
private:
    SDL_Texture* tex_shield = nullptr;
    int shield_w = 0, shield_h = 0;
    double duration = 5.0;
};

// 临时切换飞机,临时飞机生命值为1时销毁(可以无限叠加)
class TimePlayer : public MiddlePlayer
{
public:
    TimePlayer(AbstractPlayer* concrete_player);
    ~TimePlayer();

    virtual void update(double deltaTime);
    virtual void render() { player->render(); };
    virtual void increase_hp(int val) { tmp_hp += val; if(tmp_hp > tmp_max_hp) tmp_hp = tmp_max_hp; }
    virtual void decrease_hp(int val) { tmp_hp -= val; if(tmp_hp < 0) tmp_hp = 0; }
    virtual int get_current_hp() const { return tmp_hp; }
    virtual int get_max_hp() const { return tmp_max_hp; }

private:
    // 临时飞机属性
    SDL_Texture* origin_tex;
    int origin_width, origin_height;
    double origin_shoot_cd;
    SpawnBulletCallback origin_spawn_bullet;
    Bullet origin_bullet_template;
    int tmp_max_hp = 5;
    int tmp_hp = 5;         
};

// TODO:发射追踪导弹
// class RocketPlayer
// {
// }

#endif // _PLAYER_H_