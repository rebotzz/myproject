#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "core/actor.h"

class SpriteAnim;
class Enemy : public Actor
{
private:
    SpriteAnim* anim_move_ = nullptr;
    SpriteAnim* anim_hurt_ = nullptr;
    SpriteAnim* anim_dead_ = nullptr;
    SpriteAnim* current_anim_ = nullptr;

public:
    Enemy() { init(); }
    Enemy(Object *parent, const glm::vec2 &position);
    ~Enemy();
    static Enemy* createAndAddEnemyChild(Object* parent, const glm::vec2& position);

    virtual void update(float dt);

protected:
    void init();
    void updateMotion(float dt);
    void updateAnim();
};





#endif // _ENEMY_H_