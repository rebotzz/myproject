#ifndef _WEAPON__THUNDER_H_
#define _WEAPON__THUNDER_H_

#include "weapon.h"

class SpriteAnim;
class Enemy;

class WeaponThunder : public Weapon
{
private:
    SpriteAnim* anim_ = nullptr;

public:
    WeaponThunder(float damage, float attack_cd):Weapon(damage, attack_cd) {}
    ~WeaponThunder() = default;
    static WeaponThunder* createAndAddWeaponThunderChild(Object* parent, float damage, float attack_cd);

    virtual void update(float dt) override;
    virtual void enableAttack() override;
    bool attack(Enemy* target);

protected:
    void updateAnim(float dt);
};





#endif // _WEAPON__THUNDER_H_