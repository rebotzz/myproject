#include "ui_enemy_hp_bar.h"
#include "../core/actor.h"


UIEnemyHPBar::UIEnemyHPBar(Actor *parent, const glm::vec2 &size, const glm::vec2 &offset, AchorMode mode)
{
    if(parent)
    {
        setParent(parent);
        parent->safeAddChild(this);
    }
    setAchorModeAndSize(mode, size);
    setRelativeOffset(offset);
}

UIEnemyHPBar *UIEnemyHPBar::createAndAddUIHPBarChild(Actor *parent, const glm::vec2 &size, const glm::vec2 &offset, AchorMode mode)
{
    auto ui_hp_bar = new UIEnemyHPBar(parent, size, offset, mode);
    return ui_hp_bar;
}

void UIEnemyHPBar::update(float dt)
{
    // 基类更新
    UIBar::update(dt);
    // 更新血条百分比
    auto status = dynamic_cast<Actor*>(parent_)->getStatus();
    setPercentage(status->getHP() / status->getMaxHP());
}