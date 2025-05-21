#ifndef _WEAPON__THUNDER_H_
#define _WEAPON__THUNDER_H_

#include "raw/weapon.h"
#include "spell_thunder.h"

class SpriteAnim;
class Enemy;

class WeaponThunder : public Weapon
{
protected:
    
public:
    WeaponThunder(float damage, float attack_cd):Weapon(damage, attack_cd) {}
    ~WeaponThunder() = default;
    static WeaponThunder* createAndAddWeaponThunderChild(Object* parent, float damage, float attack_cd);

    virtual void update(float dt) override;
    virtual void attack(const glm::vec2& target) override;

protected:
    
};





#endif // _WEAPON__THUNDER_H_