#include "weapon.h"
#include "../affiliate/collide_box.h"
#include "../core/actor.h"
#include "status.h"
#include "../world/spell.h"

void Weapon::update(float dt)
{
    ObjectAffiliate::update(dt);
    attack_timer_ += dt;
}

void Weapon::attack(const glm::vec2& )
{
    attack_timer_ = 0.0f;
    if(attack_callback_) attack_callback_();
}

bool Weapon::canAttack() const
{
    return attack_timer_ >= attack_cd_ && dynamic_cast<Actor*>(parent_)->getStatus()->getMana() >= mana_cost_;
}
