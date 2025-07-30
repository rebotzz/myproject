#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "core/actor.h"
#include "raw/status.h"

class SpriteAnim;
class Weapon;
class Scene;
class Effect;
class Timer;


// 角色
class Player : public Actor
{
protected:
    // 动画
    SpriteAnim* anim_move_ = nullptr;
    SpriteAnim* anim_idle_ = nullptr;
    Weapon* weapon_thunder_ = nullptr;   // 武器1
    Weapon* weapon_fire_ = nullptr;      // 武器2
    Effect* effect_dead_ = nullptr;             // 特效
    Effect* effect_born_ = nullptr;
    int score_ = 0;     // 得分，暂时放在玩家类，应该放在状态类吗？还是场景类？

    // 自动逃跑
    Actor* nearest_enemy = nullptr;             // 最近敌人
    Timer* decide_timer = nullptr;              // 控制改变方向的频率
    float decide_interval = 0.5f;

    // 与敌人交换控制
    Actor* control_enemy = nullptr;

public:
    Player(Scene* parent, const glm::vec2& position);
    virtual ~Player();

    virtual bool handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;

    // getters and setters
    int getScore() const { return score_; }
    void setScore(int val) { score_ = val; }
    void addScore(int val) { score_ += val; }
    bool getIsDead() const { return status_->getIsDead(); }
    Weapon* getWeapon1() const { return weapon_thunder_; }
    Weapon* getWeapon2() const { return weapon_fire_; }

protected:
    void updateSpriteAnim();
    void autoEscape();
    void switchControlWithEnemy(Actor* enemy);
    Actor* findNearestEnemy();
    bool checkEnemyInScene(Actor* enemy);
};



#endif // _PLAYER_H_