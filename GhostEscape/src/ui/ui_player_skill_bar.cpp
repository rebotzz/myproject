#include "ui_player_skill_bar.h"

UIPlayerSkillBar::UIPlayerSkillBar(Object *parent, const glm::vec2 &position, ResID tex_icon, const glm::vec2 &icon_scale)
{
    if(parent)
    {
        parent->safeAddChild(this);
        setParent(parent);
    }
    setObjectType(ObjectType::Screen);
    setRenderPosition(position);
    sprite_icon_bg_ = Sprite::createAndAddSpriteChild(this, tex_icon, icon_scale, glm::vec2(0.0f), AchorMode::CENTER_LEFT);
    sprite_icon_fg_ = Sprite::createAndAddSpriteChild(this, tex_icon, icon_scale, glm::vec2(0.0f), AchorMode::CENTER_LEFT);
    sprite_icon_bg_->setTextureAlpha(0.5f);
}