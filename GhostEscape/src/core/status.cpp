#include "status.h"

Status *Status::createAndAddStatusChild(Object *parent, float max_hp, float max_mana, float mana_recover_cd, float invincible_cd)
{
    auto status = new Status;
    status->setHP(max_hp);
    status->setMaxHP(max_hp);
    status->setMana(max_mana);
    status->setMaxMana(max_mana);
    status->setManaRecoverCD(mana_recover_cd);
    status->setInvincibleCD(invincible_cd);
    if(parent)
    {
        parent->safeAddChild(status);
        status->setParent(parent);
    }
    return status;
}

void Status::update(float dt)
{
    mana_timer_ += dt;
    if(mana_timer_ >= mana_recover_cd_)
    {
        mana_timer_ -= mana_recover_cd_;
        mana_ += 1.0f;
        if(mana_ > max_mana_) mana_ = max_mana_;
    }

    if(invincible_)
    {
        invincible_timer_ += dt;
        if(invincible_timer_ >= invincible_cd_)
        {
            invincible_timer_ = 0.0f;
            invincible_ = false;
        }
    }
}

bool Status::takeDamage(float val)
{
    if(invincible_) return false;
    hp_ -= val;
    invincible_ = true;
    return true;
}
