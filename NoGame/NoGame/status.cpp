#include "status.h"

Status *Status::createAndAddStatusChild(Object *parent, float max_hp, float max_mana, float mana_recover_intensity, float invincible_interval)
{
    auto status = new Status;
    status->setHP(max_hp);
    status->setMaxHP(max_hp);
    status->setMana(max_mana);
    status->setMaxMana(max_mana);
    status->setManaRecoverIntensity(mana_recover_intensity);
    status->setInvincibleInterval(invincible_interval);
    if(parent)
    {
        parent->safeAddChild(status);
        status->setParent(parent);
    }
    return status;
}

void Status::update(float dt)
{
    // 发力恢复
    mana_ += mana_regen_ * dt;
    if(mana_ > max_mana_) mana_ = max_mana_;

    // 更新无敌帧
    if(invincible_)
    {
        invincible_timer_ += dt;
        if(invincible_timer_ >= invincible_interval_)
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
    if(on_hurt_) on_hurt_();
    return true;
}
