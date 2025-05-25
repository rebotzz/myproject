#ifndef _UI_PLAYER_STATUS_H_
#define _UI_PLAYER_STATUS_H_

#include "ui_player_bar.h"
#include "ui_player_skill_bar.h"


class Scene;
class HUDText;

// 玩家UI界面
// 挂载到场景
class UIPlayerStatus : public ObjectScreen
{
protected:
    UIPlayerBar* ui_hp_bar_ = nullptr;      // 血条
    UIPlayerBar* ui_mana_bar_ = nullptr;    // 蓝条
    UIPlayerSkillBar* ui_skill_bar_ = nullptr;  // 技能条
    HUDText* hud_text_ = nullptr;       // 得分

public:
    UIPlayerStatus(Scene* parent);
    ~UIPlayerStatus();
    static UIPlayerStatus* createAndAddUIPlayerStatusChild(Scene* parent);

    virtual void update(float dt);
    virtual void render();

protected:
    void updateBar();
    void updateScore();
};



#endif // _UI_PLAYER_STATUS_H_