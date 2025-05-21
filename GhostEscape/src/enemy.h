#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "core/actor.h"

class SpriteAnim;
class Enemy : public Actor
{
private:
    // 动画
    SpriteAnim* anim_move_ = nullptr;
    SpriteAnim* anim_hurt_ = nullptr;
    SpriteAnim* anim_dead_ = nullptr;
    SpriteAnim* current_anim_ = nullptr;

public:
    Enemy(Object *parent, const glm::vec2 &position);
    ~Enemy();
    static Enemy* createAndAddEnemyChild(Object* parent, const glm::vec2& position);

    virtual void update(float dt);

    void takeDamage(float damage);

protected:
    void updateMotion(float dt);
    void updateAnim();
    void updateCollide();
};





#endif // _ENEMY_H_