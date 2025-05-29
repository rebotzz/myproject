#include "weapon.h"
#include "../affiliate/collide_box.h"


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