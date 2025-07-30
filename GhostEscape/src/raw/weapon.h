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
    std::function<void()> auto_attack_callback_; // 攻击执行逻辑
    Spell* spell_protype_ = nullptr; // 技能原型
    ResID sound_id_ = ResID::None;  // 技能音效ID
    SDL_MouseButtonFlags attack_key_ = SDL_BUTTON_LEFT; // 攻击按键


public:
    Weapon(Object* parent, float damage, float attack_cd,  float mana_cost)
        :ObjectAffiliate(parent), damage_(damage), attack_cd_(attack_cd), mana_cost_(mana_cost) {}
    ~Weapon() = default;

    virtual bool handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual bool canAttack() const;
    virtual Spell* attack(const glm::vec2& position);
    void autoAttack();

    // setters and getters
    void setDamage(float val) { damage_ = val; }
    float getDamage() const { return damage_; }
    void setAttackCD(float val) { attack_cd_ = val; }
    float getAttackCDPercentage() const { return std::min(1.0f, attack_timer_ / attack_cd_); }
    void setAutoAttackCallback(const std::function<void()>& callback) { auto_attack_callback_ = callback; }
    void setAutoAttack(bool val) { auto_attack_ = val; }
    bool getAutoAttack() const { return auto_attack_; }
    void setManaCost(float val) { mana_cost_ = val; }
    float getManaCost() const { return mana_cost_; }
    void setSpellProtype(Spell* spell) { spell_protype_ = spell; }
    Spell* getSpellProtype() const { return spell_protype_; }
    void setSoundID(const ResID& id) { sound_id_ = id; }
    const ResID& getSoundID() const { return sound_id_; }
    void setAttackKey(SDL_MouseButtonFlags key) { attack_key_ = key; }
};



#endif // _WEAPON_H_