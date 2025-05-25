#include "text_lable.h"
#include "../core/object_screen.h"

TextLable::TextLable(Object* parent, const std::string& text, ResID font_id, int font_size, SDL_FColor color, AchorMode mode)
    :ObjectAffiliate(parent),text_color_(color)
{
    ttf_text_ = game_.createTTF_Text(text, game_.getAssetStore().getFont(font_id, font_size));  
    int text_w = 0, text_h = 0;
    TTF_GetTextSize(ttf_text_, &text_w, &text_h);
    setAchorModeAndSize(mode, glm::vec2(text_w, text_h));
}
TextLable::~TextLable()
{
    if(ttf_text_) TTF_DestroyText(ttf_text_);
}

void TextLable::render()
{
    game_.renderTTF_Text(ttf_text_, render_center_positon_ + offset_, text_color_);
}

void TextLable::setText(const std::string& text)
{ 
    TTF_SetTextString(ttf_text_, text.c_str(), 0); 
    int text_w = 0, text_h = 0;
    TTF_GetTextSize(ttf_text_, &text_w, &text_h);
    setSize(glm::vec2(text_w, text_h));
}

