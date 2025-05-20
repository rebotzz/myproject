#include "weapon_thunder.h"
#include "../core/sprite_anim.h"
#include "../core/collide_box.h"
#include "../enemy.h"
#include "../core/status.h"

WeaponThunder *WeaponThunder::createAndAddWeaponThunderChild(Object *parent, float damage, float attack_cd)
{
    auto weapon_thunder = new WeaponThunder(damage, attack_cd);
    weapon_thunder->anim_ = SpriteAnim::createAndAddSpriteAnimChild(weapon_thunder, ResID::Tex_ThunderstrikeWBlur, 13, 3.0f);
    weapon_thunder->collide_box_->setSize(weapon_thunder->anim_->getSize() * 3.0f);
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
    // 动画播完结束攻击碰撞盒子，可以，毕竟敌人有无敌帧
    if(anim_->getIsFinished())
    {
        collide_box_->setActive(false);
        anim_->setActive(false);
    }
}

void WeaponThunder::enableAttack()
{
    Weapon::enableAttack();
    anim_->restart();
    anim_->setActive(true);
}

bool WeaponThunder::attack(Enemy *target)
{
    if(!target->getStatus()->getIsInvincible() && collide_box_->checkCollision(target->getCollideBox()))
    {
        target->getStatus()->takeDamage(damage_);
        return true;
    }
    return false;
}

void WeaponThunder::updateAnim(float )
{
    // if(anim_)
    // anim_->update(dt);
}
