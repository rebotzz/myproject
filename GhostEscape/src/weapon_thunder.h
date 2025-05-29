#ifndef _WEAPON__THUNDER_H_
#define _WEAPON__THUNDER_H_

#include "raw/weapon.h"

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

    virtual bool handleEvent(const SDL_Event& event) override;      // 武器攻击:（可以玩家处理handleEvent，由武器处理更自动些）
    virtual bool canAttack() const override;
    virtual void attack(const glm::vec2& position) override;

protected:
    
};





#endif // _WEAPON__THUNDER_H_