#ifndef _STATUS_H_
#define _STATUS_H_

#include "object.h"

// 状态类
class Status : public Object
{
protected:
    float hp_ = 100.0f;         // 生命
    float max_hp_ = 100.0f;     
    float mana_ = 100.0f;       // 法力
    float max_mana_ = 100.0f;    
    float mana_timer_ = 0.0f;
    float mana_recover_cd_ = 0.1f;
    bool invincible_ = false;   // 无敌帧
    float invincible_timer_ = 0.0f;
    float invincible_cd_ = 0.5f;

public:
    Status() = default;
    ~Status() = default;
    static Status* createAndAddStatusChild(Object* parent, float max_hp, float max_mana, 
        float mana_recover_cd, float invincible_cd); 

    virtual void update(float dt);

    void useMana(float val) { mana_ -= val; }

    // setters and getters
    void setHP(float val) { hp_ = val; }
    float getHP() const { return hp_; }
    void setMaxHP(float val) { max_hp_ = val; }
    float getMaxHP() const { return max_hp_; }
    void setMana(float val) { mana_ = val; }
    float getMana() const { return mana_; }
    void setMaxMana(float val) { max_mana_ = val; }
    float getMaxMana() const { return max_mana_; }
    void setManaRecoverCD(float val) { mana_recover_cd_ = val; }

    void setInvincible(bool flag) { invincible_ = flag; }
    bool getIsInvincible() const { return invincible_; }
    void setInvincibleCD(float val) { invincible_cd_ = val; }
    float getInvincibleProgress() const { return invincible_timer_ / invincible_cd_; }
};





#endif // _STATUS_H_