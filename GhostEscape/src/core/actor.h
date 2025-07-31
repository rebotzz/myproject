#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "object_world.h"
#include "scene.h"
#include "../raw/status.h"

class Status;
class AffilateBar;
class MoveControl;

// 角色
class Actor : public ObjectWorld
{
protected:
    glm::vec2 velocity_ = glm::vec2(0);
    float max_speed_ = 300.0f;
    Status* status_ = nullptr;              // 状态
    AffilateBar* health_bar_ = nullptr;     // 血条
    MoveControl* move_control_ = nullptr;   // 移动控制
    bool is_sync_camera = false;

public:
    Actor() = default;
    Actor(Object* parent):ObjectWorld(parent) {};
    virtual ~Actor() {};

    virtual void update(float dt) override;
    virtual void takeDamage(float damage) { status_->takeDamage(damage); };

    // getters and setters
    float getMaxSpeed() const { return max_speed_; }
    void setMaxSpeed(float val) { max_speed_ = val; }
    Status* getStatus() const { return status_; }
    void setMoveControl(MoveControl* val);
    MoveControl* getMoveControl() const { return move_control_; }
    void setHealthBar(AffilateBar* val) { health_bar_ = val; }
    void setVelocity(const glm::vec2& val) { velocity_ = val; }
    const glm::vec2& getVelocity() const { return velocity_; }
    void removeControl();
    void setEnableSyncCamera(bool flag) { is_sync_camera = flag; }

protected:
    virtual void move(float dt);
    void updateHealthBar();
    void syncCamera(float dt);
};



#endif // _ACTOR_H_