#include "ui_player_status.h"
#include "../scene_main.h"
#include "../player.h"
#include "../raw/status.h"
#include "../raw/weapon.h"
#include "hud_text.h"
#include "../raw/weapon.h"


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
    float icon_w = 0;
    SDL_GetTextureSize(game_.getAssetStore().getTexture(tex_icon), &icon_w, nullptr);
    float bar_offset_x = icon_w * icon_scale.x * 0.7f;
    sprite_bar_bg_ = new Sprite(this, tex_bar_bg, AchorMode::CENTER_LEFT, bar_scale);
    sprite_bar_bg_->setRelativeOffset(glm::vec2(bar_offset_x, 0.0f));
    sprite_bar_fg_ = new Sprite(this, tex_bar_fg, AchorMode::CENTER_LEFT, bar_scale);
    sprite_bar_fg_->setRelativeOffset(glm::vec2(bar_offset_x, 0.0f));
    sprite_icon_ = new Sprite(this, tex_icon, AchorMode::CENTER_LEFT, icon_scale);

}

UIPlayerSkillBar::UIPlayerSkillBar(Object *parent, Weapon* weapon, const glm::vec2 &position, 
    ResID tex_icon, const glm::vec2 &icon_scale)
    :weapon_(weapon)
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

    // debug:如果在update中更新进度条，但是(觉得没必要)没有调用基类的update，那么会导致组件无法从缓冲区添加挂载
}

void UIPlayerSkillBar::updateBar()
{
    sprite_icon_fg_->setPercentage({1, weapon_->getAttackCDPercentage()});
}



UIPlayerStatus::UIPlayerStatus(Scene* parent, Weapon* weapon1, Weapon* weapon2):ObjectScreen(parent)
{
    setObjectType(ObjectType::Screen);
    ui_hp_bar_ = new UIPlayerBar(this, glm::vec2(30.f, 60.0f), ResID::Tex_RedPotion, ResID::Tex_BarBg, 
        ResID::Tex_BarRed, glm::vec2(1.0f), glm::vec2(3.0f));
    ui_mana_bar_ = new UIPlayerBar(this, glm::vec2(350.f, 60.0f), ResID::Tex_BluePotion, ResID::Tex_BarBg, 
        ResID::Tex_BarBlue, glm::vec2(1.0f), glm::vec2(3.0f));
    ui_skill_bar1_ = new UIPlayerSkillBar(this, weapon1, glm::vec2(700.f, 60.0f), ResID::Tex_ElectricIcon, glm::vec2(0.25f));
    ui_skill_bar2_ = new UIPlayerSkillBar(this, weapon2, glm::vec2(850.f, 60.0f), ResID::Tex_FireIcon, glm::vec2(0.25f));

    auto text_pos = glm::vec2(game_.getScreenSize().x - 20.f, 60.0f); 
    hud_text_ = new HUDText(this, text_pos, "Score:0", 36, ResID::Tex_Textfield01, AchorMode::CENTER_RIGHT);
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
    ui_skill_bar1_->updateBar();    
    ui_skill_bar2_->updateBar();
}

void UIPlayerStatus::updateScore()
{
    auto score = dynamic_cast<SceneMain*>(parent_)->getPlayer()->getScore();
    std::string str = "Score:" + std::to_string(score);
    hud_text_->setText(str);
}
