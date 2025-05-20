#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "../core/object_screen.h"

class CollideBox;

class Weapon : public ObjectScreen
{
protected:
    float damage_ = 10.0f;
    float attack_cd_ = 1.0f;
    float attack_timer_ = 0.0f;
    CollideBox* collide_box_ = nullptr;
    bool enable_ = true;

protected:
    Weapon(float damage, float attack_cd);
    ~Weapon() = default;
public:

    virtual void update(float dt) override;

    bool canAttack() const { return attack_timer_ >= attack_cd_; }
    virtual void enableAttack();
    virtual bool attack() { return false; }

    // setters and getters
    void setDamage(float val) { damage_ = val; }
    float getDamage() const { return damage_; }
    void setAttackCD(float val) { attack_cd_ = val; }
    CollideBox* getCollideBox() const { return collide_box_; }
    void setEnable(bool flag) { enable_ = flag; } 
    bool getEnable() const { return enable_; }

protected:
    void updateAttack();
};





#endif // _WEAPON_H_