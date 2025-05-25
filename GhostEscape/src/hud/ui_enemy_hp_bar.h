#ifndef _UI_HP_BAR_H_
#define _UI_HP_BAR_H_

#include "ui_bar.h"
#include "../core/status.h"


class Actor;

class UIEnemyHPBar : public UIBar
{
protected:

public:
    UIEnemyHPBar(Actor* parent, const glm::vec2& size, const glm::vec2 &offset, AchorMode mode);
    ~UIEnemyHPBar() = default;
    static UIEnemyHPBar* createAndAddUIHPBarChild(Actor* parent, const glm::vec2& size, 
        const glm::vec2 &offset = glm::vec2(0), AchorMode mode = AchorMode::BOTTOM_CENTER);

    virtual void update(float dt);

};


#endif // _UI_HP_BAR_H_