#include "ui_player_bar.h"

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
    auto x_offset = 40.0f;
    sprite_bar_bg_ = Sprite::createAndAddSpriteChild(this, tex_bar_bg, bar_scale, glm::vec2(x_offset, 0.0f), AchorMode::CENTER_LEFT);
    sprite_bar_fg_ = Sprite::createAndAddSpriteChild(this, tex_bar_fg, bar_scale, glm::vec2(x_offset, 0.0f), AchorMode::CENTER_LEFT);
    sprite_icon_ = Sprite::createAndAddSpriteChild(this, tex_icon, icon_scale, glm::vec2(0.0f), AchorMode::CENTER_LEFT);
}

