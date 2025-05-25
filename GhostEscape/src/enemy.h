#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "core/actor.h"

class SpriteAnim;
class Player;
class Weapon;
class Enemy : public Actor
{
    enum class EnemyState
    {
        NORMAL, HURT, DEAD
    };
private:
    // 动画与状态
    SpriteAnim* anim_move_ = nullptr;
    SpriteAnim* anim_hurt_ = nullptr;
    SpriteAnim* anim_dead_ = nullptr;
    SpriteAnim* current_anim_ = nullptr;
    EnemyState current_state_ = EnemyState::NORMAL;
    // 目标玩家与武器
    Player* target_ = nullptr;
    Weapon* weapon_ = nullptr;

public:
    Enemy(Object *parent, const glm::vec2 &position);
    ~Enemy();
    static Enemy* createAndAddEnemyChild(Object* parent, const glm::vec2& position);

    virtual void update(float dt);

    // setters and getters
    void setTarget(Player* target) { target_ = target; }
    Player* getTarget() const { return target_; }

protected:
    void updateVelocity();
    void updateState();
    void updateCollide();
    void checkAndRemove();
    void attack();
};





#endif // _ENEMY_H_