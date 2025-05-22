#ifndef _UI_TEXT_H_
#define _UI_TEXT_H_

#include "../core/object_screen.h"
#include "../core/sprite.h"

// HUD文字   屏幕对象，挂载到场景
class HUDText : public ObjectScreen
{
protected:
    Sprite* sprite_bg_ = nullptr;
    TTF_Text* text_ = nullptr;
    glm::vec2 margin_ = glm::vec2(40.f);

public:
    HUDText(Object* parent, const glm::vec2& position, const std::string& text, int font_scale, ResID tex_bg);
    virtual ~HUDText() override;
    static HUDText* createAndAddHUDTextChild(Object* parent, const glm::vec2& position, const std::string& text, int font_scale, ResID tex_bg);
    
    virtual void render() override;
    
};

#endif // _UI_TEXT_H_