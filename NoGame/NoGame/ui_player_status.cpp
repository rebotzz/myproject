#include "ui_player_status.h"
#include "../scene_main.h"
#include "../player.h"
#include "../raw/status.h"
#include "../raw/weapon.h"
#include "hud_text.h"


UIPlayerBar::UIPlayerBar(Object *parent, const glm::vec2 &position, ResID tex_icon, ResID tex_bar_bg, ResID tex_bar_fg, 
    const glm::vec2 &icon_scale, const glm::vec2 &bar_scale)
{
    if(parent)
    {
        parent->safeAddChild(this);
        setParent(parent);
    }
    setObjectType(ObjectType::Screen);
    setRenderPosition(position);
    sprite_icon_ = new Sprite(this, tex_icon, AchorMode::CENTER_LEFT, icon_scale);
    sprite_bar_bg_ = new Sprite(this, tex_bar_bg, AchorMode::CENTER_LEFT, bar_scale, glm::vec2(sprite_icon_->getSize().x * 0.7f, 0.0f));
    sprite_bar_fg_ = new Sprite(this, tex_bar_fg, AchorMode::CENTER_LEFT, bar_scale, glm::vec2(sprite_icon_->getSize().x * 0.7f, 0.0f));
}

UIPlayerSkillBar::UIPlayerSkillBar(Object *parent, const glm::vec2 &position, ResID tex_icon, const glm::vec2 &icon_scale)
{
    if(parent)
    {
        parent->safeAddChild(this);
        setParent(parent);
    }
    setObjectType(ObjectType::Screen);
    setRenderPosition(position);
    sprite_icon_bg_ = new Sprite(this, tex_icon, AchorMode::CENTER_LEFT, icon_scale);
    sprite_icon_fg_ = new Sprite(this, tex_icon, AchorMode::CENTER_LEFT, icon_scale);
    sprite_icon_bg_->setTextureAlpha(0.5f);
}

UIPlayerStatus::UIPlayerStatus(Scene* parent):ObjectScreen(parent)
{
    setObjectType(ObjectType::Screen);
    ui_hp_bar_ = new UIPlayerBar(this, glm::vec2(30.f, 60.0f), ResID::Tex_RedPotion, ResID::Tex_BarBg, 
        ResID::Tex_BarRed, glm::vec2(1.0f), glm::vec2(3.0f));
    ui_mana_bar_ = new UIPlayerBar(this, glm::vec2(350.f, 60.0f), ResID::Tex_BluePotion, ResID::Tex_BarBg, 
        ResID::Tex_BarBlue, glm::vec2(1.0f), glm::vec2(3.0f));
    ui_skill_bar_ = new UIPlayerSkillBar(this, glm::vec2(700.f, 60.0f), ResID::Tex_ElectricIcon, glm::vec2(0.23f));

    auto text_pos = glm::vec2(game_.getScreenSize().x - 20.f, 60.0f); 
    hud_text_ = new HUDText(this, text_pos, "Score:0", 36, ResID::Tex_Textfield01, AchorMode::CENTER_RIGHT);
}


UIPlayerStatus *UIPlayerStatus::createAndAddUIPlayerStatusChild(Scene *parent)
{
    return new UIPlayerStatus(parent);
}

void UIPlayerStatus::update(float dt)
{
    ObjectScreen::update(dt);
    updateBar();
    updateScore();
}

void UIPlayerStatus::render()
{
    ObjectScreen::render();
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

void UIPlayerStatus::updateScore()
{
    auto score = dynamic_cast<SceneMain*>(parent_)->getPlayer()->getScore();
    std::string str = "Score:" + std::to_string(score);
    hud_text_->setText(str);
}
