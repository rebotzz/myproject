#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "core/actor.h"
#include "raw/status.h"
#include "weapon_thunder.h"

class SpriteAnim;
class WeaponThunder;
class Scene;
class Effect;

// 角色
class Player : public Actor
{
protected:
    // 动画
    SpriteAnim* anim_move_ = nullptr;
    SpriteAnim* anim_idle_ = nullptr;
    WeaponThunder* weapon_thunder_ = nullptr;   // 武器
    Effect* effect_dead_ = nullptr;             // 特效
    Effect* effect_born_ = nullptr;
    int score_ = 0;     // 得分，暂时放在玩家类，应该放在状态类吗？还是场景类？

public:
    Player(Scene* parent, const glm::vec2& position);
    virtual ~Player();
    static Player* createAndAddPlayerChild(Scene* parent, const glm::vec2& position);

    virtual bool handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;

    // getters and setters
    float getAttackCDPercentage() const { return weapon_thunder_->getAttackCDPercentage(); }
    int getScore() const { return score_; }
    void setScore(int val) { score_ = val; }
    void addScore(int val) { score_ += val; }
    bool getIsDead() const { return status_->getIsDead(); }

protected:
    virtual void move(float dt) override;
    void updateSpriteAnim();
    // 工具函数
    void syncCamera(float dt);  // 相机跟随，放在玩家类比场景类更合理
};



#endif // _PLAYER_H_