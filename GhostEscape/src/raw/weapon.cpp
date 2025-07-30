#include "weapon.h"
#include "../affiliate/collide_box.h"
#include "../core/actor.h"
#include "status.h"
#include "../world/spell.h"
#include "../world/spell_bullet.h"


bool Weapon::handleEvent(const SDL_Event& event)
{
    if(auto_attack_) return ObjectAffiliate::handleEvent(event);

    // 技能释放
    if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == attack_key_ && canAttack())
    {
        // 不能直接SDL_获取鼠标位置，因为屏幕缩放会导致鼠标位置错位，game_.getMouseState进行鼠标位置了映射
        glm::vec2 cursor_pos;  
        game_.getMouseState(cursor_pos);
        // 世界 = 渲染 + 相机； 渲染坐标 = 世界 - 相机
        auto target = cursor_pos + game_.getCurrentScene()->getCameraPosition();
        attack(target);
        return true;
    }
    return ObjectAffiliate::handleEvent(event);
}


void Weapon::update(float dt)
{
    ObjectAffiliate::update(dt);
    attack_timer_ += dt;

    if(auto_attack_ && canAttack())
    {
        autoAttack();
    }
}

Spell* Weapon::attack(const glm::vec2& position)
{
    attack_timer_ = 0;
    dynamic_cast<Actor*>(parent_)->getStatus()->useMana(mana_cost_);
    game_.playSound(sound_id_); 
    auto spell = spell_protype_->clone();
    spell->setPosition(position);
    return spell;
}

void Weapon::autoAttack()
{
    attack_timer_ = 0.0f;
    if(auto_attack_callback_) auto_attack_callback_();
}

bool Weapon::canAttack() const
{
    return attack_timer_ >= attack_cd_ && dynamic_cast<Actor*>(parent_)->getStatus()->getMana() >= mana_cost_;
}
