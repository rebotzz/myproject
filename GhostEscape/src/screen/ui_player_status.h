#ifndef _UI_PLAYER_STATUS_H_
#define _UI_PLAYER_STATUS_H_

#include "../core/object_screen.h"
#include "../affiliate/sprite.h"

class Scene;
class HUDText;
class Weapon;

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
    void setPercentage(const glm::vec2& percentage) { sprite_bar_fg_->setPercentage(percentage); }
};

// 作为玩家状态栏的组件
class UIPlayerSkillBar : public ObjectScreen
{
protected:
    Sprite* sprite_icon_bg_ = nullptr;    
    Sprite* sprite_icon_fg_ = nullptr;    
    Weapon* weapon_ = nullptr;
    
public:
    UIPlayerSkillBar(Object* parent, Weapon* weapon, const glm::vec2& position, ResID tex_icon, const glm::vec2& icon_scale = glm::vec2(1.0f));
    ~UIPlayerSkillBar() = default;

    // setters and getters
    void updateBar();
    void setWeapon(Weapon* weapon) { weapon_ = weapon; }
};

// 玩家UI界面   挂载到场景
class UIPlayerStatus : public ObjectScreen
{
protected:
    UIPlayerBar* ui_hp_bar_ = nullptr;      // 血条
    UIPlayerBar* ui_mana_bar_ = nullptr;    // 蓝条
    UIPlayerSkillBar* ui_skill_bar1_ = nullptr;  // 技能条1
    UIPlayerSkillBar* ui_skill_bar2_ = nullptr;  // 技能条2
    HUDText* hud_text_ = nullptr;       // 得分

public:
    UIPlayerStatus(Scene* parent, Weapon* weapon1, Weapon* weapon2);
    ~UIPlayerStatus() = default;

    virtual void update(float dt);
    virtual void render();

protected:
    void updateBar();
    void updateScore();
};



#endif // _UI_PLAYER_STATUS_H_