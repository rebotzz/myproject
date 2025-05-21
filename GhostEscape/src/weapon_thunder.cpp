#include "weapon_thunder.h"
#include "core/actor.h"
#include "core/status.h"

WeaponThunder *WeaponThunder::createAndAddWeaponThunderChild(Object *parent, float damage, float attack_cd, float mana_cost)
{
    auto weapon_thunder = new WeaponThunder(damage, attack_cd, mana_cost);
    if(parent)
    {
        parent->safeAddChild(weapon_thunder);
        weapon_thunder->setParent(parent);
    }
    return weapon_thunder;
}

void WeaponThunder::update(float dt)
{
    Weapon::update(dt);
}

bool WeaponThunder::canAttack() const 
{
    // 技能冷却结束，并且法力值足够
    if(Weapon::canAttack() && dynamic_cast<Actor*>(parent_)->getStatus()->getMana() >= mana_cost_)
    {
        return true;
    }
    return false;
}

void WeaponThunder::attack(const glm::vec2& target)
{
    Weapon::attack(target);
    dynamic_cast<Actor*>(parent_)->getStatus()->useMana(mana_cost_);
    new SpellThunder(dynamic_cast<Actor*>(parent_)->getScene(), damage_, target);
}

