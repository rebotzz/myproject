#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "../affiliate/object_affiliate.h"
#include "../world/spell.h"
#include <functional>

// 改为继承附属对象，跟随玩家移动   (教程中武器继承自Object，我觉得继承自ObjectAffiliate也行，可以处理武器有动画的情况)
class Weapon : public ObjectAffiliate   
{
protected:
    float mana_cost_ = 10.0f;       // 技能消耗法力    
    float damage_ = 10.0f;
    float attack_cd_ = 1.0f;
    float attack_timer_ = 0.0f;
    bool auto_attack_ = false;
    std::function<void()> attack_callback_; // 攻击执行逻辑
    Spell* spell_protype_ = nullptr; // 技能原型
    ResID sound_id_ = ResID::None;  // 技能音效ID

public:
    Weapon(Object* parent, float damage, float attack_cd,  float mana_cost)
        :ObjectAffiliate(parent), damage_(damage), attack_cd_(attack_cd) {}
    ~Weapon() = default;

    virtual void update(float dt) override;
    virtual bool canAttack() const;
    virtual void attack(const glm::vec2& position);

    // setters and getters
    void setDamage(float val) { damage_ = val; }
    float getDamage() const { return damage_; }
    void setAttackCD(float val) { attack_cd_ = val; }
    float getAttackCDPercentage() const { return std::min(1.0f, attack_timer_ / attack_cd_); }
    void setAttackCallback(const std::function<void()>& callback) { attack_callback_ = callback; }
};



#endif // _WEAPON_H_