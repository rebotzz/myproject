#include "ui_player_status.h"
#include "../scene_main.h"
#include "../player.h"
#include "../core/status.h"
#include "../raw/weapon.h"

UIPlayerStatus::UIPlayerStatus(Scene* parent)
{
    if(parent)
    {
        parent->addChild(this);
        setParent(parent);
    }
    setObjectType(ObjectType::Screen);
    ui_hp_bar_ = new UIPlayerBar(this, glm::vec2(30.f, 60.0f), ResID::Tex_RedPotion, ResID::Tex_BarBg, 
        ResID::Tex_BarRed, glm::vec2(1.0f), glm::vec2(3.0f));
    ui_mana_bar_ = new UIPlayerBar(this, glm::vec2(400.f, 60.0f), ResID::Tex_BluePotion, ResID::Tex_BarBg, 
        ResID::Tex_BarBlue, glm::vec2(1.0f), glm::vec2(3.0f));
    ui_skill_bar_ = new UIPlayerSkillBar(this, glm::vec2(700.f, 60.0f), ResID::Tex_ElectricIcon, glm::vec2(0.3f));
}

UIPlayerStatus *UIPlayerStatus::createAndAddUIPlayerStatusChild(Scene *parent)
{
    return new UIPlayerStatus(parent);
}

void UIPlayerStatus::update(float dt)
{
    ObjectScreen::update(dt);
    updateBar();
}

void UIPlayerStatus::updateBar()
{
    // 更新进度条
    auto player = dynamic_cast<SceneMain*>(parent_)->getPlayer();
    auto status = player->getStatus();
    ui_hp_bar_->setPercentage(glm::vec2(status->getHP() / status->getMaxHP(), 1.0f));
    ui_mana_bar_->setPercentage(glm::vec2(status->getMana() / status->getMaxMana(), 1.0f));
    ui_skill_bar_->setPercentage(glm::vec2(1.0f, player->getAttackCDPercentage()));
}
