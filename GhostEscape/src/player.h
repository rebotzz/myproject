#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "core/actor.h"
#include "weapon_thunder.h"

class SpriteAnim;
class Status;
class WeaponThunder;
class Scene;

// 角色
class Player : public Actor
{
protected:
    // 动画
    SpriteAnim* anim_move_ = nullptr;
    SpriteAnim* anim_idle_ = nullptr;
    // 武器
    WeaponThunder* weapon_thunder_ = nullptr;

public:
    Player(Scene* parent, const glm::vec2& position);
    virtual ~Player();

    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;

    // 工具函数
    static Player* createAndAddPlayerChild(Scene* parent, const glm::vec2& position);

    // getters and setters
    float getAttackCDPercentage() const { return weapon_thunder_->getAttackCDPercentage(); }

protected:
    void updateKeyboardControl();
    void updateMotion(float dt);
    void updateSpriteAnim();
};



#endif // _PLAYER_H_