#ifndef _WEAPON__THUNDER_H_
#define _WEAPON__THUNDER_H_

#include "raw/weapon.h"
#include "spell_thunder.h"

class SpriteAnim;
class Enemy;

class WeaponThunder : public Weapon
{
protected:
    float mana_cost_ = 10.0f;       // 技能消耗法力    

public:
    WeaponThunder(float damage, float attack_cd, float mana_cost):Weapon(damage, attack_cd), mana_cost_(mana_cost) {}
    ~WeaponThunder() = default;
    static WeaponThunder* createAndAddWeaponThunderChild(Object* parent, float damage, float attack_cd, float mana_cost);

    virtual bool canAttack() const override;
    virtual void attack(const glm::vec2& position) override;

protected:
    
};





#endif // _WEAPON__THUNDER_H_