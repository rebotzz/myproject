#ifndef _STATUS_H_
#define _STATUS_H_

#include "../core/object.h"
#include <functional>

// 状态类
class Status : public Object
{
protected:
    float hp_ = 100.0f;         // 生命
    float max_hp_ = 100.0f;     
    float mana_ = 100.0f;       // 法力
    float max_mana_ = 100.0f;    
    float mana_regen_ = 10.f;   // 法力恢复速率
    float damage_ = 50.0f;      // 攻击力
    bool invincible_ = false;   // 无敌帧
    float invincible_timer_ = 0.0f;
    float invincible_interval_ = 0.5f;
    std::function<void()> on_hurt_;  // 受击执行另外逻辑，例如播放受击音效

public:
    Status() = default;
    ~Status() = default;
    static Status* createAndAddStatusChild(Object* parent, float max_hp, float max_mana, 
        float mana_recover_intensity, float invincible_interval); 

    virtual void update(float dt);

    void useMana(float val) { mana_ -= val; }
    bool takeDamage(float val);

    // setters and getters
    void setHP(float val) { hp_ = val; }
    float getHP() const { return hp_; }
    void setMaxHP(float val) { max_hp_ = val; }
    float getMaxHP() const { return max_hp_; }
    void setMana(float val) { mana_ = val; }
    float getMana() const { return mana_; }
    void setMaxMana(float val) { max_mana_ = val; }
    float getMaxMana() const { return max_mana_; }
    void setManaRecoverIntensity(float val) { mana_regen_ = val; }
    void setDamage(float val) { damage_ = val; }
    float getDamage() const { return damage_; }

    void setInvincible(bool flag) { invincible_ = flag; }
    bool getIsInvincible() const { return invincible_; }
    void setInvincibleInterval(float val) { invincible_interval_ = val; }
    float getInvincibleProgress() const { return invincible_timer_ / invincible_interval_; }
    bool getIsDead() const { return hp_ <= 0.0f; }
    void setOnHurtCallback(const std::function<void()>& callback) { on_hurt_ = callback; }
};





#endif // _STATUS_H_