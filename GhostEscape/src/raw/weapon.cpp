#include "weapon.h"
#include "../core/collide_box.h"


Weapon::Weapon(float damage, float attack_cd):damage_(damage), attack_cd_(attack_cd)
{
}

void Weapon::update(float dt)
{
    ObjectAffiliate::update(dt);
    attack_timer_ += dt;
}

