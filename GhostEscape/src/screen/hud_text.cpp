#include "hud_text.h"
#include <sstream>

HUDText::HUDText(Object *parent, const glm::vec2 &position, const std::string &text, int font_scale, ResID tex_bg, 
    AchorMode mode)
    :ObjectScreen(parent)
{
    setRenderPosition(position);
    sprite_bg_ = new Sprite(this, tex_bg, mode);
    // 文字都显示在图片中间，所以使用默认CENTER锚点
    text_lable_ = new TextLable(this, text, ResID::Font_VonwaonBitmap16px, font_scale); 
    setMargin(margin_scale_);
}

HUDText *HUDText::createAndAddHUDTextChild(Object *parent, const glm::vec2 &position, const std::string &text, 
    int font_scale, ResID tex_bg, AchorMode mode)
{
    auto hud_text = new HUDText(parent, position, text, font_scale, tex_bg, mode);
    return hud_text;
}


void HUDText::setText(const std::string &text)
{
    text_lable_->setText(text);
    updateBackgroundSpriteSize();
}

void HUDText::setMargin(const glm::vec2 margin_scale)
{
    auto boundary = text_lable_->getSize() * margin_scale;
    if(boundary.x > max_margin_size_.x || boundary.y > max_margin_size_.y)
    {
        margin_scale_ = max_margin_size_ / text_lable_->getSize();
    }
    else
    {
        margin_scale_ = margin_scale;
    }
    updateBackgroundSpriteSize();
}

void HUDText::updateBackgroundSpriteSize()
{
    // 确保无论是中心锚点还是左右锚点，文本都在背景图中间，偏移是整体的
    auto boundary = text_lable_->getSize() * margin_scale_;
    sprite_bg_->setSize(text_lable_->getSize() + boundary);
    text_lable_->setRenderCenterPosition(sprite_bg_->getLeftTopPosition()
    + sprite_bg_->getSize() / 2.0f);
}

void HUDText::setSpriteBackground(ResID tex_id)
{
    AchorMode achor_mode = AchorMode::CENTER;
    if(sprite_bg_) 
    {
        achor_mode = sprite_bg_->getAchorMode();
        // 从子节点中移除
        sprite_bg_->setCanRemove(true); // debug:不直接delete sprite_bg_;
        sprite_bg_ = nullptr;
    }

    sprite_bg_ = new Sprite(this, tex_id, achor_mode);
    updateBackgroundSpriteSize();
}

