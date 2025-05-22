#include "ui_text.h"
#include <sstream>

HUDText::HUDText(Object *parent, const glm::vec2 &position, const std::string &text, int font_scale, ResID tex_bg)
    :ObjectScreen(parent)
{
    setRenderPosition(position);

    std::string line = text.substr(0, text.find("\n"));
    auto font = game_.getAssetStore().getFont(ResID::Font_VonwaonBitmap16px, font_scale);
    int line_w = 0, line_h = 0;
    TTF_GetStringSize(font, line.c_str(), 0, &line_w, &line_h);

    text_ = game_.createText(text, font, line_w);
    int text_w = 0, text_h = 0;
    TTF_GetTextSize(text_, &text_w, &text_h);
    sprite_bg_ = Sprite::createAndAddSpriteChild(this, tex_bg);
    sprite_bg_->setSize(glm::vec2(text_w, text_h) + margin_);
}

HUDText::~HUDText()
{
    if(text_) TTF_DestroyText(text_);
}

HUDText *HUDText::createAndAddHUDTextChild(Object *parent, const glm::vec2 &position, const std::string &text, int font_scale, ResID tex_bg)
{
    auto hud_text = new HUDText(parent, position, text, font_scale, tex_bg);
    return hud_text;
}

void HUDText::render()
{
    ObjectScreen::render();
    game_.renderText(text_, sprite_bg_->getRenderPosition() + margin_ / 2.0f);
}