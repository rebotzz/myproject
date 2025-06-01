#include "weapon_thunder.h"
#include "core/actor.h"
#include "raw/status.h"
#include "world/spell.h"

WeaponThunder *WeaponThunder::createAndAddWeaponThunderChild(Object *parent, float damage, float attack_cd, float mana_cost)
{
    auto weapon_thunder = new WeaponThunder(damage, attack_cd, mana_cost);
    if(parent)
    {
        parent->safeAddChild(weapon_thunder);
        weapon_thunder->setParent(parent);
    }
    return weapon_thunder;
}

bool WeaponThunder::handleEvent(const SDL_Event& event)
{
    if(Weapon::handleEvent(event)) return true;
    bool event_handled = false;
    // 技能释放
    switch(event.type)
    {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if(event.button.button == SDL_BUTTON_LEFT && canAttack()) 
        {
            // 不能直接SDL_获取鼠标位置，因为屏幕缩放会导致鼠标位置错位，game_.getMouseState进行鼠标位置了映射
            glm::vec2 cursor_pos;  
            game_.getMouseState(cursor_pos);
            // 世界 = 渲染 + 相机； 渲染坐标 = 世界 - 相机
            auto target = cursor_pos + game_.getCurrentScene()->getCameraPosition();
            attack(target);
            event_handled = true;
        }
        break;
    }
    return event_handled;
}

bool WeaponThunder::canAttack() const 
{
    // 技能冷却结束，并且法力值足够
    if(Weapon::canAttack() && dynamic_cast<Actor*>(parent_)->getStatus()->getMana() >= mana_cost_)
    {
        return true;
    }
    return false;
}

void WeaponThunder::attack(const glm::vec2& position)
{
    Weapon::attack(position);
    dynamic_cast<Actor*>(parent_)->getStatus()->useMana(mana_cost_);
    game_.playSound(ResID::Sound_BigThunder); 
    auto spell_thunder = new Spell(game_.getCurrentScene(), damage_, position, CollideShape::Circle, 
        ResID::Tex_ThunderstrikeWBlur, 13, glm::vec2(3));
    spell_thunder->getCollideBox()->setHitLayer(CollideLayer::Enemy);
}

