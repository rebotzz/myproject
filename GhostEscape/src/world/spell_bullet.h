#ifndef _SPELL_BULLET_H_
#define _SPELL_BULLET_H_

#include "spell.h"

// 子弹
class SpellBullet : public Spell
{
    glm::vec2 direction_ = glm::vec2(0);
    float max_speed_ = 200.f;

public:
    SpellBullet(Object* parent, float damage, const glm::vec2& init_position, ResID tex_id, int frame_count, 
        const glm::vec2& scale = glm::vec2(1), float frame_interval = 0.1f);

    virtual SpellBullet* clone() const override;

    // setters and getters
    void setDirection(const glm::vec2& direction);
    const glm::vec2& getDirection() const { return direction_; }
    void setMaxSpeed(float val) { max_speed_ = val; }
    float getMaxSpeed() const { return max_speed_; }

protected:
    virtual void move(float dt) override;
    virtual void checkAndRemove() override;
};




#endif // _SPELL_BULLET_H_