#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "../core/object_affiliate.h"

// 改为继承附属对象，跟随玩家移动
class Weapon : public ObjectAffiliate   
{
protected:
    float damage_ = 10.0f;
    float attack_cd_ = 1.0f;
    float attack_timer_ = 0.0f;

protected:
    Weapon(float damage, float attack_cd);
    ~Weapon() = default;
public:
    virtual void update(float dt) override;

    bool canAttack() const { return attack_timer_ >= attack_cd_; }
    virtual void attack(const glm::vec2& ) { attack_timer_ = 0.0f; }

    // setters and getters
    void setDamage(float val) { damage_ = val; }
    float getDamage() const { return damage_; }
    void setAttackCD(float val) { attack_cd_ = val; }
    float getAttackCDPercentage() const { return std::min(1.0f, attack_timer_ / attack_cd_); }
};



#endif // _WEAPON_H_