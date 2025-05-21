#include "weapon_thunder.h"
#include "core/actor.h"

WeaponThunder *WeaponThunder::createAndAddWeaponThunderChild(Object *parent, float damage, float attack_cd)
{
    auto weapon_thunder = new WeaponThunder(damage, attack_cd);
    if(parent)
    {
        parent->addChild(weapon_thunder);
        weapon_thunder->setParent(parent);
    }
    return weapon_thunder;
}

void WeaponThunder::update(float dt)
{
    Weapon::update(dt);
}

void WeaponThunder::attack(const glm::vec2& target)
{
    Weapon::attack(target);
    new SpellThunder(dynamic_cast<Actor*>(parent_)->getScene(), damage_, target);
}

