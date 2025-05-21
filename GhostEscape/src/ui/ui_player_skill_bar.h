#ifndef _UI_PLAYER_SKILL_BAR_H_
#define _UI_PLAYER_SKILL_BAR_H_

#include "../core/object_screen.h"
#include "../core/sprite.h"

// 作为玩家状态栏的组件
class UIPlayerSkillBar : public ObjectScreen
{
protected:
    Sprite* sprite_icon_bg_ = nullptr;    
    Sprite* sprite_icon_fg_ = nullptr;    

public:
    UIPlayerSkillBar(Object* parent, const glm::vec2& position, ResID tex_icon, const glm::vec2& icon_scale = glm::vec2(1.0f));
    ~UIPlayerSkillBar() = default;

    // setters and getters
    void setPercentage(const glm::vec2& percentage) { sprite_icon_fg_->setRenderPercentage(percentage); }
};




#endif // _UI_PLAYER_SKILL_BAR_H_