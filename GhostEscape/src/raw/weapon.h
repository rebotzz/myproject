#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "../affiliate/object_affiliate.h"
#include <functional>

// 改为继承附属对象，跟随玩家移动
class Weapon : public ObjectAffiliate   
{
protected:
    float damage_ = 10.0f;
    float attack_cd_ = 1.0f;
    float attack_timer_ = 0.0f;
    std::function<void()> attack_callback_; // 攻击执行逻辑

public:
    Weapon(float damage, float attack_cd):damage_(damage), attack_cd_(attack_cd) {}
    Weapon(Object* parent, float damage, float attack_cd):ObjectAffiliate(parent), damage_(damage), attack_cd_(attack_cd) {}
    ~Weapon() = default;

    virtual void update(float dt) override;
    virtual bool canAttack() const { return attack_timer_ >= attack_cd_; }
    virtual void attack(const glm::vec2& position);

    // setters and getters
    void setDamage(float val) { damage_ = val; }
    float getDamage() const { return damage_; }
    void setAttackCD(float val) { attack_cd_ = val; }
    float getAttackCDPercentage() const { return std::min(1.0f, attack_timer_ / attack_cd_); }
    void setAttackCallback(const std::function<void()>& callback) { attack_callback_ = callback; }
};



#endif // _WEAPON_H_