#ifndef _UI_PLAYER_STATUS_H_
#define _UI_PLAYER_STATUS_H_

#include "ui_player_bar.h"
#include "ui_player_skill_bar.h"

class Scene;

// 玩家UI界面
// 挂载到场景
class UIPlayerStatus : public ObjectScreen
{
protected:
    UIPlayerBar* ui_hp_bar_ = nullptr;
    UIPlayerBar* ui_mana_bar_ = nullptr;
    UIPlayerSkillBar* ui_skill_bar_ = nullptr;

public:
    UIPlayerStatus(Scene* parent);
    ~UIPlayerStatus() = default;
    static UIPlayerStatus* createAndAddUIPlayerStatusChild(Scene* parent);

    virtual void update(float dt);

protected:
    void updateBar();
};



#endif // _UI_PLAYER_STATUS_H_