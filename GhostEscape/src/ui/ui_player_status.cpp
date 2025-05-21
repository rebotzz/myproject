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

    text_ = game_.createText("Score:", game_.getAssetStore().getFont(ResID::Font_VonwaonBitmap16px, 48), 0);
}

UIPlayerStatus::~UIPlayerStatus()
{
    TTF_DestroyText(text_);
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

    // 渲染得分
    game_.renderText(text_, text_position_);
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
    TTF_SetTextString(text_, str.c_str(), 0);
    int text_w = 0, text_h = 0; 
    TTF_GetTextSize(text_, &text_w, &text_h);
    text_position_ = glm::vec2(game_.getScreenSize().x - text_w - 50.f, 30.0f);
}
