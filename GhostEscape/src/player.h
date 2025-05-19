#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "core/actor.h"

// 角色
class Player : public Actor
{
protected:

public:
    Player();
    virtual ~Player();

    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;

    // 工具函数
    static Player* createAndAddPlayerChild(Object* parent, glm::vec2 position);

protected:
    void updateKeyboardControl();
    void updateMotion(float dt);
    void updateSpriteAnim();
};



#endif // _PLAYER_H_