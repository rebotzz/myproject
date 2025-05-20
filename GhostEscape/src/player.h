#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "core/actor.h"

class SpriteAnim;
class Status;

// 角色
class Player : public Actor
{
protected:
    // 动画
    SpriteAnim* anim_move_ = nullptr;
    SpriteAnim* anim_idle_ = nullptr;
    // 状态
    Status* status_ = nullptr;


public:
    Player();
    virtual ~Player();

    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;

    // 工具函数
    static Player* createAndAddPlayerChild(Object* parent, glm::vec2 position);

    // getters and setters
    Status* getStatus() const { return status_; }

protected:
    void updateKeyboardControl();
    void updateMotion(float dt);
    void updateSpriteAnim();
};



#endif // _PLAYER_H_