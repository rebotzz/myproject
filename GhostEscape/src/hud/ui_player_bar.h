#ifndef _UI_PLAYER_BAR_H_
#define _UI_PLAYER_BAR_H_

#include "../core/object_screen.h"
#include "../affiliate/sprite.h"

// 作为玩家状态栏的组件
class UIPlayerBar : public ObjectScreen
{
protected:
    Sprite* sprite_icon_ = nullptr;    
    Sprite* sprite_bar_bg_ = nullptr;    
    Sprite* sprite_bar_fg_ = nullptr;    

public:
    UIPlayerBar(Object* parent, const glm::vec2& position, ResID tex_icon, ResID tex_bg, ResID tex_fg, 
        const glm::vec2& icon_scale = glm::vec2(1.0f), const glm::vec2& bar_scale = glm::vec2(1.0f));
    ~UIPlayerBar() = default;

    // setters and getters
    void setPercentage(const glm::vec2& percentage) { sprite_bar_fg_->setRenderPercentage(percentage); }
};




#endif // _UI_PLAYER_BAR_H_