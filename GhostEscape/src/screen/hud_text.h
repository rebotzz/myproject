#ifndef _HUD_TEXT_H_
#define _HUD_TEXT_H_

#include "../core/object_screen.h"
#include "../affiliate/sprite.h"
#include "../affiliate/text_lable.h"

// HUD文字   屏幕对象，挂载到场景
class HUDText : public ObjectScreen
{
protected:
    Sprite* sprite_bg_ = nullptr;
    TextLable* text_lable_ = nullptr;
    glm::vec2 margin_scale_ = glm::vec2(0.35f, 0.35f);  // 背景框边缘留白比例
    glm::vec2 max_margin_size_ = glm::vec2(40);         // 背景框边缘留白像素尺寸

public:
    HUDText(Object* parent, const glm::vec2& position, const std::string& text, int font_scale, ResID tex_bg, 
        AchorMode mode = AchorMode::CENTER);
    static HUDText* createAndAddHUDTextChild(Object* parent, const glm::vec2& position, const std::string& text, 
        int font_scale, ResID tex_bg, AchorMode mode = AchorMode::CENTER);
    
    virtual void render() override;
    
    // setters and getters
    void setText(const std::string& text);
    void setMargin(const glm::vec2 margin_scale);
    void setSpriteBackground(ResID tex_id);

protected:
    void updateBackgroundSpriteSize();
};

#endif // _HUD_TEXT_H_